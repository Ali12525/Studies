package Server;

import javax.swing.*;
import javax.swing.UIManager.*;
import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.concurrent.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import MyPackage.CommandData;
import MyPackage.RecIntegral;

public class ServerApp {
    private static final int PORT = 12345;
    private final List<ClientInfo> clientInfos = Collections.synchronizedList(new ArrayList<>());
    private DatagramSocket socket;

    public static void main(String[] args) {
        setLookAndFeel();
        setUtf8Output();
        
        ServerApp app = new ServerApp();
        
        Runtime.getRuntime().addShutdownHook(new Thread(app::shutdown));
        
        app.startServer();

        SwingUtilities.invokeLater(() -> {
            Frame frame = new Frame(app);
            frame.setTitle("Вычисление интегралов");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setVisible(true);
        });
    }
    
    /**
     * Запускаем поток, который прослушивает UDP-порт для регистрации клиентов.
     */
    private void startServer() {
        new Thread(() -> {
            try {
                socket = new DatagramSocket(PORT);
                System.out.println("Сервер запущен на порту " + PORT);
                byte[] buf = new byte[4096];
                while (true) {
                    DatagramPacket packet = new DatagramPacket(buf, buf.length);
                    try {
                        socket.receive(packet);
                    } catch (SocketTimeoutException e) {
                        continue;
                    } catch (SocketException e) {
                        if (socket.isClosed()) {
                            System.out.println("Сокет закрыт, завершаем работу потока приёма.");
                            break;
                        } else {
                            e.printStackTrace();
                        }
                    }
                    ByteArrayInputStream bais = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
                    ObjectInputStream ois = new ObjectInputStream(bais);
                    Object obj = ois.readObject();
                    if (obj instanceof CommandData) {
                        CommandData cmd = (CommandData) obj;
                        if ("register".equals(cmd.getCommandType())) {
                            ClientInfo client = new ClientInfo(packet.getAddress(), packet.getPort());
                            if (!clientInfos.contains(client)) {
                                clientInfos.add(client);
                                System.out.println("Клиент зарегистрирован: " + packet.getAddress());
                            }
                        }
                    }
                }
            } catch (IOException | ClassNotFoundException e) {
                e.printStackTrace();
            }
        }).start();
    }
    
    /**
     * Распределяет вычислительную задачу между всеми зарегистрированными клиентами.
     */
    public double distributeTasks(double lowLim, double upLim, double widthLim) {
        if (clientInfos.isEmpty()) {
            System.out.println("Нет зарегистрированных клиентов.");
            return 0.0;
        }
        
        int numberOfClients = clientInfos.size();
        double intervalWidth = (upLim - lowLim) / numberOfClients;
        ExecutorService executor = Executors.newFixedThreadPool(numberOfClients);
        List<Future<Double>> futures = new ArrayList<>();
        
        for (int i = 0; i < numberOfClients; i++) {
            double low = lowLim + i * intervalWidth;
            double high = low + intervalWidth;
            ClientInfo client = clientInfos.get(i);
            futures.add(executor.submit(() -> sendTaskToClient(client, low, high, widthLim)));
        }
        
        double totalResult = futures.stream().mapToDouble(future -> {
            try {
                return future.get();
            } catch (Exception e) {
                e.printStackTrace();
                return 0.0;
            }
        }).sum();
        
        executor.shutdown();
        return totalResult;
    }
    
    /**
     * Отправка задачи конкретному клиенту и получение результата.
     */
    private double sendTaskToClient(ClientInfo client, double low, double high, double width) {
        int maxAttempts = 3;
        int attempts = 0;
        boolean taskAckReceived = false;

        while (attempts < maxAttempts && !taskAckReceived) {
            try {
                // Отправка задачи клиенту
                CommandData task = new CommandData("calculate", new RecIntegral(low, high, width));
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                ObjectOutputStream oos = new ObjectOutputStream(baos);
                oos.writeObject(task);
                byte[] data = baos.toByteArray();
                DatagramPacket packet = new DatagramPacket(data, data.length, client.getAddress(), client.getPort());
                socket.send(packet);

                
                // Ожидание подтверждения получения задачи
                byte[] ackBuf = new byte[4096];
                DatagramPacket ackPacket = new DatagramPacket(ackBuf, ackBuf.length);
                socket.setSoTimeout(1000);
                socket.receive(ackPacket);

                if (!ackPacket.getAddress().equals(client.getAddress()) || ackPacket.getPort() != client.getPort()) {
                    continue;
                }

                ByteArrayInputStream ackBais = new ByteArrayInputStream(ackPacket.getData(), 0, ackPacket.getLength());
                ObjectInputStream ackOis = new ObjectInputStream(ackBais);
                CommandData ackData = (CommandData) ackOis.readObject();
                if ("taskReceived".equals(ackData.getCommandType())) {
                    taskAckReceived = true;
                    System.out.println("Клиент подтвердил получение задачи.");

                    // Отправка подтверждения клиенту о получении подтверждения
                    CommandData taskAck = new CommandData("taskAcknowledged", (RecIntegral) null);
                    ByteArrayOutputStream taskAckBaos = new ByteArrayOutputStream();
                    ObjectOutputStream taskAckOos = new ObjectOutputStream(taskAckBaos);
                    taskAckOos.writeObject(taskAck);
                    byte[] taskAckData = taskAckBaos.toByteArray();
                    DatagramPacket taskAckPacket = new DatagramPacket(taskAckData, taskAckData.length, client.getAddress(), client.getPort());
                    socket.send(taskAckPacket);
                }
            } catch (SocketTimeoutException e) {
                attempts++;
                System.err.println("Подтверждение не получено, попытка " + attempts);
            } catch (IOException | ClassNotFoundException e) {
                System.err.println("Ошибка при отправке задачи клиенту: " + client.getAddress());
                e.printStackTrace();
                return 0.0;
            }
        }

        if (!taskAckReceived) {
            System.err.println("Не удалось получить подтверждение от клиента " + client.getAddress());
            return 0.0;
        }

        // Ожидание результата от клиента
        try {
            long startTime = System.currentTimeMillis();
            long timeout = 10000;
            while (System.currentTimeMillis() - startTime < timeout) {
                byte[] resultBuf = new byte[4096];
                DatagramPacket resultPacket = new DatagramPacket(resultBuf, resultBuf.length);
                socket.setSoTimeout((int) (timeout - (System.currentTimeMillis() - startTime)));
                try {
                    socket.receive(resultPacket);
                } catch (SocketTimeoutException e) {
                    throw new SocketTimeoutException("Таймаут получения результата.");
                }

                if (!resultPacket.getAddress().equals(client.getAddress()) || resultPacket.getPort() != client.getPort()) {
                    continue;
                }

                ByteArrayInputStream resultBais = new ByteArrayInputStream(resultPacket.getData(), 0, resultPacket.getLength());
                ObjectInputStream resultOis = new ObjectInputStream(resultBais);
                CommandData resultData = (CommandData) resultOis.readObject();
                if ("result".equals(resultData.getCommandType())) {
                    double result = resultData.getResIntegral();

                    // Отправка подтверждения клиенту о получении результата
                    CommandData resultAck = new CommandData("resultReceived", (RecIntegral) null);
                    ByteArrayOutputStream ackResBaos = new ByteArrayOutputStream();
                    ObjectOutputStream ackResOos = new ObjectOutputStream(ackResBaos);
                    ackResOos.writeObject(resultAck);
                    byte[] ackResData = ackResBaos.toByteArray();
                    DatagramPacket ackResPacket = new DatagramPacket(ackResData, ackResData.length, client.getAddress(), client.getPort());
                    socket.send(ackResPacket);

                    return result;
                } else {
                    System.err.println("Ожидался результат, получена команда: " + resultData.getCommandType());
                }
            }
            throw new SocketTimeoutException("Таймаут получения результата.");
        } catch (SocketTimeoutException e) {
            System.err.println("Не удалось получить результат от клиента " + client.getAddress());
        } catch (IOException | ClassNotFoundException e) {
            System.err.println("Ошибка при получении результата: " + e.getMessage());
            e.printStackTrace();
        }
        return 0.0;
    }

    
    private static void setLookAndFeel() {
        try {
            for (LookAndFeelInfo info : UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException | InstantiationException |
                 IllegalAccessException | UnsupportedLookAndFeelException ex) {
            Logger.getLogger(ServerApp.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    private static void setUtf8Output() {
        System.setOut(new PrintStream(System.out, true, StandardCharsets.UTF_8));
        System.setErr(new PrintStream(System.err, true, StandardCharsets.UTF_8));
    }
    
    /**
     * Завершает работу сервера, уведомляя всех клиентов.
     */
    public void shutdown() {
        System.out.println("Сервер завершает работу. Уведомление клиентов.");
        // Отправляем команду "exit" всем зарегистрированным клиентам
        for (ClientInfo client : clientInfos) {
            try {
                CommandData exitCmd = new CommandData("exit", (RecIntegral) null);
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                ObjectOutputStream oos = new ObjectOutputStream(baos);
                oos.writeObject(exitCmd);
                oos.flush();
                byte[] data = baos.toByteArray();
                DatagramPacket packet = new DatagramPacket(data, data.length, client.getAddress(), client.getPort());
                socket.send(packet);
            } catch (IOException ex) {
                System.err.println("Не удалось отправить команду завершения клиенту: " + client.getAddress());
                ex.printStackTrace();
            }
        }
        if (socket != null && !socket.isClosed()) {
            socket.close();
        }
    }
}   