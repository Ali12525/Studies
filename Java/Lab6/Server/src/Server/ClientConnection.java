package Server;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

/**
 * Класс для управления соединением с клиентом.
 * Обеспечивает обмен данными между сервером и клиентом через сокетное соединение.
 */
public class ClientConnection {
    private Socket socket;
    private ObjectOutputStream oos;
    private ObjectInputStream ois;
    
    /**
     * Создает новое соединение с клиентом.
     *
     * @param socket сокет для соединения с клиентом
     */
    public ClientConnection(Socket socket) throws IOException {
        this.socket = socket;
        this.oos = new ObjectOutputStream(socket.getOutputStream());
        this.ois = new ObjectInputStream(socket.getInputStream());
    }
    
    /**
     * Возвращает выходной поток для отправки данных клиенту.
     *
     * @return объект ObjectOutputStream для сериализованной отправки данных
     */
    public ObjectOutputStream getOos() {
        return oos;
    }

    /**
     * Возвращает входной поток для получения данных от клиента.
     *
     * @return объект ObjectInputStream для десериализации полученных данных
     */
    public ObjectInputStream getOis() {
        return ois;
    }
    
    /**
     * Возвращает сокет соединения с клиентом.
     *
     * @return объект Socket, представляющий соединение с клиентом
     */
    public Socket getSocket() {
        return socket;
    }
}