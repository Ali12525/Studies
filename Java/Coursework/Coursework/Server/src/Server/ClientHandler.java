package Server;

import java.io.*;
import java.net.*;
import java.util.*;
import java.nio.file.*;

public class ClientHandler implements Runnable {
    private Socket socket;
    private ObjectInputStream ois;
    private ObjectOutputStream oos;
    // После успешного логина/регистрации запомним имя пользователя
    private String username = null;

    public ClientHandler(Socket socket) {
        this.socket = socket;
        try {
            oos = new ObjectOutputStream(socket.getOutputStream());
            oos.flush();
            ois = new ObjectInputStream(socket.getInputStream());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        try {
            while (true) {
                String command = (String) ois.readObject();
                System.out.println("Получена команда: " + command);
                if (command.equals("REGISTER")) {
                    handleRegister();
                } else if (command.equals("LOGIN")) {
                    handleLogin();
                } else {
                    if (username == null) {
                        sendResponse("ERROR: Not authenticated");
                        continue;
                    }
                    switch (command) {
                        case "UPLOAD":
                            handleUpload();
                            break;
                        case "DOWNLOAD":
                            handleDownload();
                            break;
                        case "CREATE_FOLDER":
                            handleCreateFolder();
                            break;
                        case "DELETE":
                            handleDelete();
                            break;
                        case "SEARCH":
                            handleSearch();
                            break;
                        case "COPY":
                            handleCopy();
                            break;
                        case "RENAME":
                            handleRename();
                            break;
                        case "SORT":
                            handleSort();
                            break;
                        case "MOVE":
                            handleMove();
                            break;
                        case "LIST":
                            handleListFiles();
                            break;
                        default:
                            sendResponse("ERROR: Unknown command");
                    }
                }
            }
        } catch (Exception ex) {
            System.out.println("Клиент отключился: " + socket.getInetAddress());
        } finally {
            try {
                socket.close();
            } catch (Exception e) {}
        }
    }
    
    // Регистрация: ожидаем username и password
    private void handleRegister() throws IOException, ClassNotFoundException {
        String user = (String) ois.readObject();
        String pass = (String) ois.readObject();
        Map<String, String> db = Server.getUserDb();
        if (db.containsKey(user)) {
            sendResponse("ERROR: User already exists");
        } else {
            db.put(user, pass);
            username = user;
            // Создаем каталог для пользователя
            new File(Server.getBaseDir() + File.separator + username).mkdirs();
            sendResponse("OK");
        }
    }
    
    // Логин: ожидаем username и password
    private void handleLogin() throws IOException, ClassNotFoundException {
        String user = (String) ois.readObject();
        String pass = (String) ois.readObject();
        Map<String, String> db = Server.getUserDb();
        if (db.containsKey(user) && db.get(user).equals(pass)) {
            username = user;
            sendResponse("OK");
        } else {
            sendResponse("ERROR: Invalid credentials");
        }
    }
    
    // Получаем каталог текущего пользователя
    private File getUserDir() {
        return new File(Server.getBaseDir() + File.separator + username);
    }
    
    // Загрузка файла на сервер
    private void handleUpload() throws IOException, ClassNotFoundException {
        // Клиент сначала отправляет относительный путь (включая имя файла)
        String filePath = (String) ois.readObject();
        long fileSize = (long) ois.readObject();
        File outFile = new File(getUserDir(), filePath);
        // Создаем родительские каталоги, если их нет
        outFile.getParentFile().mkdirs();
        FileOutputStream fos = new FileOutputStream(outFile);
        byte[] buffer = new byte[4096];
        long remaining = fileSize;
        while (remaining > 0) {
            int read = ois.read(buffer, 0, (int) Math.min(buffer.length, remaining));
            if (read == -1)
                break;
            fos.write(buffer, 0, read);
            remaining -= read;
        }
        fos.close();
        sendResponse("OK");
    }
    
    // Скачивание файла с сервера
    private void handleDownload() throws IOException, ClassNotFoundException {
        String filePath = (String) ois.readObject();
        File file = new File(getUserDir(), filePath);
        if (!file.exists() || file.isDirectory()) {
            sendResponse("ERROR: File not found");
            return;
        }
        sendResponse("OK");
        // Отправляем имя файла и размер
        oos.writeObject(file.getName());
        oos.writeObject(file.length());
        FileInputStream fis = new FileInputStream(file);
        byte[] buffer = new byte[4096];
        int count;
        while ((count = fis.read(buffer)) > 0) {
            oos.write(buffer, 0, count);
        }
        oos.flush();
        fis.close();
    }
    
    // Создание папки
    private void handleCreateFolder() throws IOException, ClassNotFoundException {
        String folderPath = (String) ois.readObject(); // относительный путь для создания папки
        File folder = new File(getUserDir(), folderPath);
        boolean created = folder.mkdirs();
        sendResponse(created ? "OK" : "ERROR: Unable to create folder");
    }
    
    // Удаление файла или папки (рекурсивное)
    private void handleDelete() throws IOException, ClassNotFoundException {
        String path = (String) ois.readObject();
        File file = new File(getUserDir(), path);
        boolean deleted = deleteRecursively(file);
        sendResponse(deleted ? "OK" : "ERROR: Deletion failed");
    }
    
    private boolean deleteRecursively(File file) {
        if (file.isDirectory()) {
            File[] files = file.listFiles();
            if (files != null) {
                for (File child : files) {
                    deleteRecursively(child);
                }
            }
        }
        return file.delete();
    }
    
    // Поиск файлов по имени (рекурсивно)
    private void handleSearch() throws IOException, ClassNotFoundException {
        String query = (String) ois.readObject();
        List<String> results = new ArrayList<>();
        searchFiles(getUserDir(), query, results, getUserDir().getAbsolutePath().length());
        oos.writeObject(results);
        oos.flush();
    }
    
    private void searchFiles(File dir, String query, List<String> results, int baseLength) {
        File[] files = dir.listFiles();
        if (files == null)
            return;
        for (File f : files) {
            if (f.getName().contains(query)) {
                // Возвращаем относительный путь
                results.add(f.getAbsolutePath().substring(baseLength + 1));
            }
            if (f.isDirectory()) {
                searchFiles(f, query, results, baseLength);
            }
        }
    }
    
    // Копирование файла или папки
    private void handleCopy() throws IOException, ClassNotFoundException {
        String sourcePath = (String) ois.readObject();
        String destPath = (String) ois.readObject();
        File source = new File(getUserDir(), sourcePath);
        File dest = new File(getUserDir(), destPath);
        boolean success = false;
        if (source.isDirectory()) {
            success = copyDirectory(source, dest);
        } else if (source.isFile()) {
            dest.getParentFile().mkdirs();
            success = copyFile(source, dest);
        }
        sendResponse(success ? "OK" : "ERROR: Copy failed");
    }
    
    private boolean copyFile(File source, File dest) {
        try (InputStream in = new FileInputStream(source);
             OutputStream out = new FileOutputStream(dest)) {
            byte[] buffer = new byte[4096];
            int len;
            while ((len = in.read(buffer)) > 0) {
                out.write(buffer, 0, len);
            }
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }
    
    private boolean copyDirectory(File source, File dest) {
        if (!dest.exists())
            dest.mkdirs();
        File[] files = source.listFiles();
        if (files == null)
            return false;
        for (File file : files) {
            File destFile = new File(dest, file.getName());
            if (file.isDirectory()) {
                if (!copyDirectory(file, destFile))
                    return false;
            } else {
                if (!copyFile(file, destFile))
                    return false;
            }
        }
        return true;
    }
    
    // Переименование файла или папки
    private void handleRename() throws IOException, ClassNotFoundException {
        String oldPath = (String) ois.readObject();
        String newName = (String) ois.readObject();
        File oldFile = new File(getUserDir(), oldPath);
        File newFile = new File(oldFile.getParentFile(), newName);
        boolean renamed = oldFile.renameTo(newFile);
        sendResponse(renamed ? "OK" : "ERROR: Rename failed");
    }
    
    // Сортировка. В данном примере сортировка производится только в каталоге пользователя (без рекурсии)
    private void handleSort() throws IOException, ClassNotFoundException {
        String criterion = (String) ois.readObject(); // "name", "size", "type", "date"
        boolean ascending = (Boolean) ois.readObject();
        File dir = getUserDir();
        File[] files = dir.listFiles();
        if (files == null)
            files = new File[0];
        List<File> fileList = new ArrayList<>(Arrays.asList(files));
        Comparator<File> comparator = null;
        switch (criterion) {
            case "name":
                comparator = Comparator.comparing(File::getName, String.CASE_INSENSITIVE_ORDER);
                break;
            case "size":
                comparator = Comparator.comparingLong(File::length);
                break;
            case "type":
                comparator = Comparator.comparing(file -> {
                    int dot = file.getName().lastIndexOf('.');
                    return dot == -1 ? "" : file.getName().substring(dot + 1);
                }, String.CASE_INSENSITIVE_ORDER);
                break;
            case "date":
                comparator = Comparator.comparingLong(File::lastModified);
                break;
        }
        if (comparator != null) {
            if (!ascending)
                comparator = comparator.reversed();
            fileList.sort(comparator);
        }
        List<String> result = new ArrayList<>();
        for (File f : fileList) {
            result.add(f.getName());
        }
        oos.writeObject(result);
        oos.flush();
    }
    
    // Перемещение файла или папки
    private void handleMove() throws IOException, ClassNotFoundException {
        String sourcePath = (String) ois.readObject();
        String destPath = (String) ois.readObject();
        File source = new File(getUserDir(), sourcePath);
        File dest = new File(getUserDir(), destPath);
        dest.mkdirs();
        boolean success = source.renameTo(new File(dest, source.getName()));
        sendResponse(success ? "OK" : "ERROR: Move failed");
    }
    
    // Обработка запроса на просмотр содержимого папки
    private void handleListFiles() throws IOException, ClassNotFoundException {
        // Получаем относительный путь к папке, содержимое которой нужно вернуть
        String relativePath = (String) ois.readObject();
        File folder = new File(getUserDir(), relativePath);
        if (!folder.exists() || !folder.isDirectory()) {
            oos.writeObject("ERROR: Folder not found");
            return;
        }
        // Получаем список имен файлов/папок внутри каталога
        String[] fileNames = folder.list();
        List<String> list = new ArrayList<>();
        if (fileNames != null) {
            for (String name : fileNames) {
                list.add(name);
            }
        }
        oos.writeObject("OK");
        oos.writeObject(list);
        oos.flush();
    }
    
    private void sendResponse(String response) throws IOException {
        oos.writeObject(response);
        oos.flush();
    }
}