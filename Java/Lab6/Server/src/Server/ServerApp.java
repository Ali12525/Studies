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
    private final List<ClientConnection> clientConnections = Collections.synchronizedList(new ArrayList<>());

    public static void main(String[] args) {
        setLookAndFeel();
        setUtf8Output();
        
        ServerApp app = new ServerApp();
        
        // Регистрируем shutdown hook для корректного завершения сервера, когда пользователь закрывает окно
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            app.shutdown();
        }));
        
        app.startServer();

        SwingUtilities.invokeLater(() -> {
            Frame frame = new Frame(app);
            frame.setTitle("Вычисление интегралов");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setVisible(true);
        });
    }
    
    /**
     * Запуск серверного потока для приёма подключений клиентов.
     */
    private void startServer() {
        new Thread(() -> {
            try (ServerSocket serverSocket = new ServerSocket(PORT)) {
                System.out.println("Сервер запущен на порту " + PORT);
                while (true) {
                    Socket clientSocket = serverSocket.accept();
                    try {
                        ClientConnection connection = new ClientConnection(clientSocket);
                        clientConnections.add(connection);
                        System.out.println("Клиент подключен: " + clientSocket.getInetAddress());
                    } catch (IOException ex) {
                        System.err.println("Ошибка при создании соединения с клиентом.");
                        ex.printStackTrace();
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }
    
    /**
     * Распределение вычислительной задачи по всем подключенным клиентам.
     *
     * @param lowLim начальное значение интегрирования
     * @param upLim конечное значение интегрирования
     * @param widthLim шаг вычисления
     * @return суммарный результат интегрирования
     */
    public double distributeTasks(double lowLim, double upLim, double widthLim) {
        if (clientConnections.isEmpty()) {
            System.out.println("Нет подключенных клиентов.");
            return 0.0;
        }
        
        int numberOfClients = clientConnections.size();
        double intervalWidth = (upLim - lowLim) / numberOfClients;
        ExecutorService executor = Executors.newFixedThreadPool(numberOfClients);
        List<Future<Double>> futures = new ArrayList<>();
        
        for (int i = 0; i < numberOfClients; i++) {
            double low = lowLim + i * intervalWidth;
            double high = low + intervalWidth;
            ClientConnection connection = clientConnections.get(i);
            futures.add(executor.submit(() -> sendTaskToClient(connection, low, high, widthLim)));
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
     * Отправка задачи на вычисление конкретному клиенту и получение результата.
     *
     * @param connection соединение с клиентом
     * @param low        нижняя граница интервала
     * @param high       верхняя граница интервала
     * @param width      шаг интегрирования
     * @return результат вычисления или 0.0 в случае ошибки
     */
    private double sendTaskToClient(ClientConnection connection, double low, double high, double width) {
        try {
            ObjectOutputStream oos = connection.getOos();
            ObjectInputStream ois = connection.getOis();
            CommandData task = new CommandData("calculate", new RecIntegral(low, high, width));
            
            oos.writeObject(task);
            oos.flush();
            
            CommandData result = (CommandData) ois.readObject();
            if ("result".equals(result.getCommandType())) {
                return result.getResIntegral();
            }
        } catch (IOException | ClassNotFoundException e) {
            System.err.println("Ошибка при отправке задачи клиенту.");
            e.printStackTrace();
        }
        return 0.0;
    }
    
    /**
     * Установка Nimbus Look and Feel.
     */
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
    
    /**
     * Настройка вывода в консоль с использованием UTF-8.
     */
    private static void setUtf8Output() {
        System.setOut(new PrintStream(System.out, true, StandardCharsets.UTF_8));
        System.setErr(new PrintStream(System.err, true, StandardCharsets.UTF_8));
    }
    
    /**
    * Отключает сервер, уведомляя всех подключенных клиентов о завершении работы
    * и закрывая соответствующие соединения.
    */
    public void shutdown() {
        System.out.println("Сервер завершает работу. Уведомление клиентов.");
        for (ClientConnection connection : clientConnections) {
            try {
                ObjectOutputStream oos = connection.getOos();
                oos.writeObject(new CommandData("exit", (RecIntegral) null));
                oos.flush();
            } catch (IOException ex) {
                System.err.println("Не удалось отправить команду завершения клиенту.");
                ex.printStackTrace();
            } finally {
                connection.close();
            }
        }
    }

}