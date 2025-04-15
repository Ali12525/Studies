package Server;

import java.io.*;
import java.net.*;
import java.io.File;
import java.nio.charset.StandardCharsets;

public class Server {
    private static final int PORT = 12345;
    private static final String BASE_DIR = "server_data/catalogs";
    private static UserDao userDao;

    public static void main(String[] args) {
        setUtf8Output();
        new File(BASE_DIR).mkdirs();
        userDao = new UserDao();
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Сервер запущен на порту " + PORT);
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Подключился клиент: " + clientSocket.getInetAddress());
                new Thread(new ClientHandler(clientSocket)).start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public static String getBaseDir() {
        return BASE_DIR;
    }
    
    private static void setUtf8Output() {
        System.setOut(new PrintStream(System.out, true, StandardCharsets.UTF_8));
        System.setErr(new PrintStream(System.err, true, StandardCharsets.UTF_8));
    }
}