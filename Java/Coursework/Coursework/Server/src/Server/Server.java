package Server;

import java.io.*;
import java.net.*;
import java.util.*;
import java.io.File;
import java.nio.charset.StandardCharsets;

public class Server {
    private static final int PORT = 12345;
    // Простейшая «база данных» пользователей (username -> password)
    private static Map<String, String> userDb = Collections.synchronizedMap(new HashMap<>());
    // Базовая директория для файлов пользователей
    private static final String BASE_DIR = "server_data";

    public static void main(String[] args) {
        // Создаем базовую папку, если ее нет
        setUtf8Output();
        
        new File(BASE_DIR).mkdirs();
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
    
    public static Map<String, String> getUserDb() {
        return userDb;
    }
    
    public static String getBaseDir() {
        return BASE_DIR;
    }
    
    private static void setUtf8Output() {
        System.setOut(new PrintStream(System.out, true, StandardCharsets.UTF_8));
        System.setErr(new PrintStream(System.err, true, StandardCharsets.UTF_8));
    }
}