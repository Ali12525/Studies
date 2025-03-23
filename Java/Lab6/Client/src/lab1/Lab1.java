package lab1;

import java.io.*;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public class Lab1 {

    public static void main(String[] args) {
        System.setOut(new PrintStream(System.out, true, StandardCharsets.UTF_8));
        System.setErr(new PrintStream(System.err, true, StandardCharsets.UTF_8));
        
        // Адрес и порт сервера
        String serverAddress = "localhost";
        int serverPort = 12345;

        try (Socket socket = new Socket(serverAddress, serverPort);
             ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
             ObjectInputStream ois = new ObjectInputStream(socket.getInputStream())) {

            System.out.println("Подключено к серверу.");

            while (true) {
                // Получаем задачу от сервера
                CommandData task = (CommandData) ois.readObject();
                System.out.println("Задача получена: " + task);

                // Проверяем тип команды
                if ("calculate".equals(task.getCommandType())) {
                    // Вычисляем интеграл на заданном интервале
                    DistributedIntegralCalculator calculator = new DistributedIntegralCalculator(task.getLowLim(), task.getUpLim(), task.getWidthLim(), 1);
                    double result = calculator.calculate();
                    
                    System.out.println("Результат вычислен " + result);

                    // Отправляем результат серверу
                    oos.writeObject(new CommandData("result", result));
                    oos.flush();
                }
            }
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
            System.out.println("Ошибка");
        }
    }
}