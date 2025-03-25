package Client;

import java.io.*;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

import MyPackage.CommandData;
import java.net.SocketException;

public class Main {
    public static void main(String[] args) {
        System.setOut(new PrintStream(System.out, true, StandardCharsets.UTF_8));
        System.setErr(new PrintStream(System.err, true, StandardCharsets.UTF_8));
        final int PORT = 12345;
        final String SERVER_ADDRES = "localhost";
        final int COUNT_THREAD = 1;

        try (Socket socket = new Socket(SERVER_ADDRES, PORT);
             ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
             ObjectInputStream ois = new ObjectInputStream(socket.getInputStream())) {

            System.out.println("Подключено к серверу.");

            while (true) {
                Object obj = ois.readObject();
                if (!(obj instanceof CommandData)) {
                    System.out.println("Ошибка: получен неизвестный объект");
                    break;
                }

                CommandData task = (CommandData) obj;
                System.out.println("Задача получена: " + task);

                if ("exit".equals(task.getCommandType())) {
                    System.out.println("Получена команда выхода. Завершение работы...");
                    break;
                }

                if ("calculate".equals(task.getCommandType())) {
                    DistributedIntegralCalculator calculator = new DistributedIntegralCalculator(task.getRecIntegral(), COUNT_THREAD);
                    double result = calculator.calculate();

                    System.out.println("Результат вычислен: " + result);

                    oos.writeObject(new CommandData("result", result));
                    oos.flush();
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