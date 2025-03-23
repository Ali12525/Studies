package lab1;

import java.io.*;
import java.net.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ClientHandler implements Runnable {
        private final Socket clientSocket;

        ClientHandler(Socket socket) {
            this.clientSocket = socket;
        }

        @Override
        public void run() {
            try (BufferedReader in = new BufferedReader(
                    new InputStreamReader(clientSocket.getInputStream()));
                 PrintWriter out = new PrintWriter(
                         clientSocket.getOutputStream(), true)) {
                
                String clientMessage;
                while ((clientMessage = in.readLine()) != null) {
                    System.out.println("Получено от клиента: " + clientMessage);
                    out.println("Эхо: " + clientMessage);
                }
            } catch (IOException e) {
                System.err.println("Ошибка обработки клиента: " + e.getMessage());
            } finally {
                try {
                    clientSocket.close();
                } catch (IOException e) {
                    System.err.println("Ошибка закрытия сокета: " + e.getMessage());
                }
                System.out.println("Клиент отключен.");
            }
        }
    }