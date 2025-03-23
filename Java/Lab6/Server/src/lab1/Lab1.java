package lab1;

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Lab1 {
    private static final int PORT = 12345;
    private static final int MAX_CLIENTS = 1;
//    private static List<Socket> clientSockets = new ArrayList<>();
//    /**
//     * @param args the command line arguments
//     */
//    public static void main(String[] args) {
//        // TODO code application logic here
//        Frame frame = new Frame(clientSockets);
//        frame.setTitle("Вычисление интегралов");
//        frame.setVisible(true);
//        
//        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
//            System.out.println("Сервер запущен. Ожидание клиентов...");
//
//            // Поток для принятия подключений клиентов
//            new Thread(() -> {
//                while (true) {
//                    try {
//                        Socket clientSocket = serverSocket.accept();
//                        synchronized (clientSockets) {
//                            clientSockets.add(clientSocket);
//                        }
//                        System.out.println("Клиент подключен: " + clientSocket.getInetAddress());
//                    } catch (IOException e) {
//                        e.printStackTrace();
//                    }
//                }
//            }).start();
//
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//    }
}
        
        
        
        
        
//        ExecutorService pool = Executors.newFixedThreadPool(MAX_CLIENTS);
//        
//        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
//            System.out.println("Сервер запущен на порту " + PORT);
//            
//            while (true) {
//                Socket clientSocket = serverSocket.accept();
//                pool.execute(() -> handleClient(clientSocket));
//            }
//        } catch (IOException e) {
//            System.err.println("Ошибка сервера: " + e.getMessage());
//        } finally {
//            pool.shutdown();
//        }
   
   
    
//    private static void handleClient(Socket clientSocket) {
//        try (ObjectOutputStream oos = new ObjectOutputStream(clientSocket.getOutputStream());
//             ObjectInputStream ois = new ObjectInputStream(clientSocket.getInputStream())) {
//            
//            oos.flush();
//            CommandData task;
//
//            while ((task = tasksQueue.poll()) != null) {
//                oos.writeObject(task);
//                oos.flush();
//
//                CommandData result = (CommandData) ois.readObject();
//                if ("result".equals(result.getCommandType())) {
//                    addToTotal(result.getResIntegral());
//                }
//            }
//            
//            oos.writeObject(new CommandData("no_task", 0));
//            oos.flush();
//        } catch (EOFException e) {
//            System.out.println("Клиент отключился");
//        } catch (IOException | ClassNotFoundException e) {
//            e.printStackTrace();
//        } finally {
//            try {
//                clientSocket.close();
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//        }
//    }