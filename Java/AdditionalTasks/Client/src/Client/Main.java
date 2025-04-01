package Client;

import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import MyPackage.CommandData;
import MyPackage.RecIntegral;

public class Main {
    public static void main(String[] args) {
        System.setOut(new PrintStream(System.out, true, StandardCharsets.UTF_8));
        System.setErr(new PrintStream(System.err, true, StandardCharsets.UTF_8));
        final int SERVER_PORT = 12345;
        final String SERVER_ADDRESS = "localhost";
        final int COUNT_THREAD = 2;
        
        try (DatagramSocket socket = new DatagramSocket()) {
            CommandData registerCmd = new CommandData("register", (RecIntegral) null);
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            ObjectOutputStream oos = new ObjectOutputStream(baos);
            oos.writeObject(registerCmd);
            oos.flush();
            byte[] regData = baos.toByteArray();
            DatagramPacket regPacket = new DatagramPacket(regData, regData.length, InetAddress.getByName(SERVER_ADDRESS), SERVER_PORT);
            socket.send(regPacket);
            
            while (true) {
                DatagramPacket packet = null;
                while (true) {
                    try {
                        byte[] buf = new byte[4096];
                        packet = new DatagramPacket(buf, buf.length);
                        socket.setSoTimeout(50000);
                        socket.receive(packet);
                        System.out.println("Данные получены...");

                        break;

                    } catch (SocketTimeoutException e) {
                        System.err.println("Сервер не отправил данные. Повторная попытка...");
                        continue;
                    } catch (IOException e) {
                        e.printStackTrace();
                        break;
                    }
                }
                
                ByteArrayInputStream bais = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
                ObjectInputStream ois = new ObjectInputStream(bais);
                CommandData task = (CommandData) ois.readObject();
                System.out.println("Задача получена: " + task);
                
                if ("exit".equals(task.getCommandType())) {
                    System.out.println("Получена команда выхода. Завершение работы...");
                    break;
                }
                
                if ("calculate".equals(task.getCommandType())) {
                    // Отправляем подтверждение получения задачи
                    CommandData ack = new CommandData("taskReceived", (RecIntegral) null);
                    ByteArrayOutputStream ackBaos = new ByteArrayOutputStream();
                    ObjectOutputStream ackOos = new ObjectOutputStream(ackBaos);
                    ackOos.writeObject(ack);
                    ackOos.flush();
                    byte[] ackData = ackBaos.toByteArray();
                    DatagramPacket ackPacket = new DatagramPacket(ackData, ackData.length, packet.getAddress(), packet.getPort());

                    int maxAckAttempts = 5;
                    int ackAttempts = 0;
                    boolean taskAcknowledged = false;

                    while (ackAttempts < maxAckAttempts && !taskAcknowledged) {
                        try {
                            socket.send(ackPacket);
                            System.out.println("Отправляем подтверждение получения задачи: " + ack);

                            // Ждем ответа от сервера
                            byte[] ackResponseBuf = new byte[4096];
                            DatagramPacket ackResponsePacket = new DatagramPacket(ackResponseBuf, ackResponseBuf.length);
                            socket.setSoTimeout(500);
                            socket.receive(ackResponsePacket);

                            // Проверяем подтверждение подтверждения
                            ByteArrayInputStream ackBais = new ByteArrayInputStream(ackResponsePacket.getData(), 0, ackResponsePacket.getLength());
                            ObjectInputStream ackOis = new ObjectInputStream(ackBais);
                            CommandData ackResponse = (CommandData) ackOis.readObject();

                            if ("taskAcknowledged".equals(ackResponse.getCommandType())) {
                                taskAcknowledged = true;
                                System.out.println("Сервер подтвердил получение нашего подтверждения");
                            }
                        } catch (SocketTimeoutException e) {
                            ackAttempts++;
                            System.err.println("Таймаут подтверждения задачи, попытка " + ackAttempts);
                        }
                    }

                    if (!taskAcknowledged) {
                        System.err.println("Не получили подтверждение задачи после " + maxAckAttempts + " попыток. Пропускаем задачу.");
                        continue;
                    }
                    
                    DistributedIntegralCalculator calculator = new DistributedIntegralCalculator(task.getRecIntegral(), COUNT_THREAD);
                    double result = calculator.calculate();
                    System.out.println("Результат вычислен: " + result);
                    
                    CommandData resultCmd = new CommandData("result", result);
                    ByteArrayOutputStream resBaos = new ByteArrayOutputStream();
                    ObjectOutputStream resOos = new ObjectOutputStream(resBaos);
                    resOos.writeObject(resultCmd);
                    resOos.flush();
                    byte[] resData = resBaos.toByteArray();
                    DatagramPacket responsePacket = new DatagramPacket(resData, resData.length, packet.getAddress(), packet.getPort());
                    
                    int maxAttempts = 5;
                    int attempts = 0;
                    boolean resultAckReceived = false;
                    while (attempts < maxAttempts && !resultAckReceived) {
                        socket.send(responsePacket);
                        try {
                            byte[] ackResBuf = new byte[4096];
                            DatagramPacket ackResPacket = new DatagramPacket(ackResBuf, ackResBuf.length);
                            socket.setSoTimeout(1000);
                            socket.receive(ackResPacket);
                            ByteArrayInputStream ackResBais = new ByteArrayInputStream(ackResPacket.getData(), 0, ackResPacket.getLength());
                            ObjectInputStream ackResOis = new ObjectInputStream(ackResBais);
                            CommandData ackRes = (CommandData) ackResOis.readObject();
                            if ("resultReceived".equals(ackRes.getCommandType())) {
                                System.out.println("Сервер подтвердил получение результата.");
                                resultAckReceived = true;
                                break;
                            }
                        } catch (SocketTimeoutException e) {
                            attempts++;
                            System.err.println("Подтверждение получения результата не получено, повторная отправка результата, попытка " + attempts);
                        }
                    }
                    if (!resultAckReceived) {
                        System.err.println("Не удалось получить подтверждение получения результата от сервера после " + maxAttempts + " попыток.");
                    }
                }
            }
        } catch (SocketException e) {
            System.out.println("Соединение с сервером разорвано.");
        } catch (EOFException e) {
            System.out.println("Сервер завершил соединение.");
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
            System.out.println("Ошибка соединения.");
        }
    }
}