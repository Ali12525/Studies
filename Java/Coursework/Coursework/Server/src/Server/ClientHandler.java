package Server;

import java.io.*;
import java.net.*;
import java.util.*;
import java.nio.file.*;
import DTO.*;

public class ClientHandler implements Runnable {
    private Socket socket;
    private ObjectInputStream ois;
    private ObjectOutputStream oos;
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
                Object obj = ois.readObject();
                if (!(obj instanceof RequestDTO)) {
                    sendResponse(new ResponseDTO(false, "Invalid request type"));
                    continue;
                }
                RequestDTO request = (RequestDTO)obj;
                System.out.println("Получена команда: " + request.getCommand());
                if (username == null && 
                   !(request instanceof RegisterRequest) && 
                   !(request instanceof LoginRequest)) {
                    sendResponse(new ResponseDTO(false, "Not authenticated"));
                    continue;
                }
                switch(request.getCommand()){
                    case REGISTER:
                        handleRegister((RegisterRequest) request);
                        break;
                    case LOGIN:
                        handleLogin((LoginRequest) request);
                        break;
                    case UPLOAD:
                        handleUpload((UploadRequest) request);
                        break;
                    case DOWNLOAD:
                        handleDownload((DownloadRequest) request);
                        break;
                    case CREATE_FOLDER:
                        handleCreateFolder((CreateFolderRequest) request);
                        break;
                    case DELETE:
                        handleDelete((DeleteRequest) request);
                        break;
                    case SEARCH:
                        handleSearch((SearchRequest) request);
                        break;
                    case COPY:
                        handleCopy((CopyRequest) request);
                        break;
                    case RENAME:
                        handleRename((RenameRequest) request);
                        break;
                    case MOVE:
                        handleMove((MoveRequest) request);
                        break;
                    case LIST_FILES:
                        handleListFiles((ListFilesRequest) request);
                        break;
                    default:
                        sendResponse(new ResponseDTO(false, "Unknown command"));
                }
            }
        } catch (Exception ex) {
            System.out.println("Клиент отключился: " + socket.getInetAddress());
        } finally {
            try { socket.close(); } catch(Exception e) {}
        }
    }
    
    // Регистрация: ожидаем username и password
    private void handleRegister(RegisterRequest req) throws IOException {
        String user = req.getUsername();
        String pass = req.getPassword();
        Map<String, String> db = Server.getUserDb();
        if (db.containsKey(user)) {
            sendResponse(new ResponseDTO(false, "User already exists"));
        } else {
            db.put(user, pass);
            username = user;
            // Создаем каталог для пользователя
            new File(Server.getBaseDir() + File.separator + username).mkdirs();
            sendResponse(new ResponseDTO(true, "OK"));
        }
    }
    
    // Логин: ожидаем username и password
    private void handleLogin(LoginRequest req) throws IOException {
        String user = req.getUsername();
        String pass = req.getPassword();
        Map<String, String> db = Server.getUserDb();
        if (db.containsKey(user) && db.get(user).equals(pass)) {
            username = user;
            sendResponse(new ResponseDTO(true, "OK"));
        } else {
            sendResponse(new ResponseDTO(false, "Invalid credentials"));
        }
    }
    
    // Получаем каталог текущего пользователя
    private File getUserDir() {
        return new File(Server.getBaseDir() + File.separator + username);
    }
    
    // Потоковая загрузка файла: читаем данные напрямую из потока
    private void handleUpload(UploadRequest req) throws IOException {
        String destPath = req.getDestinationPath();
        long fileSize = req.getFileSize();
        File outFile = new File(getUserDir(), destPath);
        outFile.getParentFile().mkdirs();
        FileOutputStream fos = new FileOutputStream(outFile);
        byte[] buffer = new byte[4096];
        long remaining = fileSize;
        while (remaining > 0) {
            int read = ois.read(buffer, 0, (int)Math.min(buffer.length, remaining));
            if (read == -1)
                break;
            fos.write(buffer, 0, read);
            remaining -= read;
        }
        fos.close();
        sendResponse(new ResponseDTO(true, "OK"));
    }
    
    // Потоковое скачивание файла: сначала отправляем размер файла, затем его содержимое
    private void handleDownload(DownloadRequest req) throws IOException {
        String filePath = req.getFilePath();
        File file = new File(getUserDir(), filePath);
        if (!file.exists() || file.isDirectory()) {
            sendResponse(new ResponseDTO(false, "File not found"));
            return;
        }
        long fileSize = file.length();
        sendResponse(new ResponseDTO(true, "OK", fileSize));
        FileInputStream fis = new FileInputStream(file);
        byte[] buffer = new byte[4096];
        int bytesRead;
        while ((bytesRead = fis.read(buffer)) != -1) {
            oos.write(buffer, 0, bytesRead);
        }
        oos.flush();
        fis.close();
    }
    
    // Создание папки
    private void handleCreateFolder(CreateFolderRequest req) throws IOException {
        String folderPath = req.getFolderPath();
        File folder = new File(getUserDir(), folderPath);
        boolean created = folder.mkdirs();
        sendResponse(new ResponseDTO(created, created ? "OK" : "Unable to create folder"));
    }
    
    // Удаление файла или папки (рекурсивное)
     private void handleDelete(DeleteRequest req) throws IOException {
        String path = req.getPath();
        File file = new File(getUserDir(), path);
        boolean deleted = deleteRecursively(file);
        sendResponse(new ResponseDTO(deleted, deleted ? "OK" : "Deletion failed"));
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
    private void handleSearch(SearchRequest req) throws IOException {
        String query = req.getQuery();
        List<String> results = new ArrayList<>();
        searchFiles(getUserDir(), query, results, getUserDir().getAbsolutePath().length());
        sendResponse(new ResponseDTO(true, "OK", results));
    }
    
    private void searchFiles(File dir, String query, List<String> results, int baseLength) {
        File[] files = dir.listFiles();
        if (files == null)
            return;
        for (File f : files) {
            if (f.getName().contains(query)) {
                results.add(f.getAbsolutePath().substring(baseLength + 1));
            }
            if (f.isDirectory()) {
                searchFiles(f, query, results, baseLength);
            }
        }
    }
    
    // Копирование файла или папки
    private void handleCopy(CopyRequest req) throws IOException {
        String sourcePath = req.getSourcePath();
        String destPath = req.getDestinationPath();
        File source = new File(getUserDir(), sourcePath);
        File dest = new File(getUserDir(), destPath);

        // Автопереименование, если цель уже существует
        if (dest.exists()) {
            dest = getAvailableName(dest);
        }

        boolean success = false;
        if (source.isDirectory()) {
            success = copyDirectory(source, dest);
        } else if (source.isFile()) {
            dest.getParentFile().mkdirs();
            success = copyFile(source, dest);
        }

        sendResponse(new ResponseDTO(success, success ? "Copied as: " + dest.getName() : "Copy failed"));
    }
    
    private File getAvailableName(File original) {
        String name = original.getName();
        File parent = original.getParentFile();

        String baseName;
        String extension = "";

        int dotIndex = name.lastIndexOf('.');
        if (original.isFile() && dotIndex > 0) {
            baseName = name.substring(0, dotIndex);
            extension = name.substring(dotIndex); // includes the dot
        } else {
            baseName = name;
        }

        int index = 1;
        File candidate;
        do {
            String newName = baseName + " (" + index + ")" + extension;
            candidate = new File(parent, newName);
            index++;
        } while (candidate.exists());

        return candidate;
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
    private void handleRename(RenameRequest req) throws IOException {
        String oldPath = req.getOldPath();
        String newName = req.getNewName();
        File oldFile = new File(getUserDir(), oldPath);
        File newFile = new File(oldFile.getParentFile(), newName);
        boolean renamed = oldFile.renameTo(newFile);
        sendResponse(new ResponseDTO(renamed, renamed ? "OK" : "Rename failed"));
    }
    
    // Перемещение файла или папки
    private void handleMove(MoveRequest req) throws IOException {
        String sourcePath = req.getSourcePath();
        String destPath = req.getDestinationPath();
        File source = new File(getUserDir(), sourcePath);
        File dest = new File(getUserDir(), destPath);
        dest.mkdirs();
        boolean success = source.renameTo(new File(dest, source.getName()));
        sendResponse(new ResponseDTO(success, success ? "OK" : "Move failed"));
    }
    
    // Обработка запроса на просмотр содержимого папки
    private void handleListFiles(ListFilesRequest req) throws IOException {
        String relativePath = req.getRelativePath();
        File folder = new File(getUserDir(), relativePath);
        if (!folder.exists() || !folder.isDirectory()) {
            sendResponse(new ResponseDTO(false, "Folder not found"));
            return;
        }
        String[] fileNames = folder.list();
        List<String> list = new ArrayList<>();
        if (fileNames != null) {
            Collections.addAll(list, fileNames);
        }
        sendResponse(new ResponseDTO(true, "OK", list));
    }
    
     // Отправка ответа клиенту
    private void sendResponse(ResponseDTO response) throws IOException {
        oos.writeObject(response);
        oos.flush();
    }
}