package client;

import java.io.*;
import java.net.*;
import java.util.List;

public class FileManagerClient {
    private String serverIp;
    private int serverPort;
    private Socket socket;
    private ObjectOutputStream oos;
    private ObjectInputStream ois;
    
    public FileManagerClient(String ip, int port) {
        this.serverIp = ip;
        this.serverPort = port;
        connect();
    }
    
    private void connect() {
        try {
            socket = new Socket(serverIp, serverPort);
            oos = new ObjectOutputStream(socket.getOutputStream());
            oos.flush();
            ois = new ObjectInputStream(socket.getInputStream());
        } catch(Exception ex) {
            ex.printStackTrace();
        }
    }
    
    public boolean register(String username, String password) {
        try {
            oos.writeObject("REGISTER");
            oos.writeObject(username);
            oos.writeObject(password);
            oos.flush();
            String response = (String) ois.readObject();
            return "OK".equals(response);
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }
    
    public boolean login(String username, String password) {
        try {
            oos.writeObject("LOGIN");
            oos.writeObject(username);
            oos.writeObject(password);
            oos.flush();
            String response = (String) ois.readObject();
            return "OK".equals(response);
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }
    
    public boolean uploadFile(File file, String destPath) {
        try {
            oos.writeObject("UPLOAD");
            // Отправляем относительный путь для сохранения файла
            oos.writeObject(destPath);
            oos.writeObject(file.length());
            FileInputStream fis = new FileInputStream(file);
            byte[] buffer = new byte[4096];
            int count;
            while((count = fis.read(buffer)) > 0) {
                oos.write(buffer, 0, count);
            }
            fis.close();
            oos.flush();
            String response = (String) ois.readObject();
            return "OK".equals(response);
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }
    
    public boolean downloadFile(String filePath, File saveTo) {
        try {
            oos.writeObject("DOWNLOAD");
            oos.writeObject(filePath);
            oos.flush();
            String response = (String) ois.readObject();
            if(!"OK".equals(response))
                return false;
            // Сервер отправляет имя файла и его размер
            String fileName = (String) ois.readObject();
            long fileSize = (long) ois.readObject();
            FileOutputStream fos = new FileOutputStream(saveTo);
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
            return true;
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }
    
    public boolean createFolder(String folderPath) {
        try {
            oos.writeObject("CREATE_FOLDER");
            oos.writeObject(folderPath);
            oos.flush();
            String response = (String) ois.readObject();
            return "OK".equals(response);
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }
    
    public boolean delete(String path) {
        try {
            oos.writeObject("DELETE");
            oos.writeObject(path);
            oos.flush();
            String response = (String) ois.readObject();
            return "OK".equals(response);
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }
    
    public List<String> search(String query) {
        try {
            oos.writeObject("SEARCH");
            oos.writeObject(query);
            oos.flush();
            List<String> results = (List<String>) ois.readObject();
            return results;
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return null;
    }
    
    public boolean copy(String source, String destination) {
        try {
            oos.writeObject("COPY");
            oos.writeObject(source);
            oos.writeObject(destination);
            oos.flush();
            String response = (String) ois.readObject();
            return "OK".equals(response);
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }
    
    public boolean rename(String oldPath, String newName) {
        try {
            oos.writeObject("RENAME");
            oos.writeObject(oldPath);
            oos.writeObject(newName);
            oos.flush();
            String response = (String) ois.readObject();
            return "OK".equals(response);
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }
    
    public List<String> sort(String criterion, boolean ascending) {
        try {
            oos.writeObject("SORT");
            oos.writeObject(criterion);
            oos.writeObject(ascending);
            oos.flush();
            List<String> sortedList = (List<String>) ois.readObject();
            return sortedList;
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return null;
    }
    
    public boolean move(String source, String destination) {
        try {
            oos.writeObject("MOVE");
            oos.writeObject(source);
            oos.writeObject(destination);
            oos.flush();
            String response = (String) ois.readObject();
            return "OK".equals(response);
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }
    
    // Метод для запроса содержимого папки по относительному пути
    public List<String> listFolder(String relativePath) {
        try {
            oos.writeObject("LIST");
            oos.writeObject(relativePath);
            oos.flush();
            String response = (String) ois.readObject();
            if (!"OK".equals(response)) {
                return null;
            }
            List<String> folderContents = (List<String>) ois.readObject();
            return folderContents;
        } catch(Exception ex) {
            ex.printStackTrace();
        }
        return null;
    }
}