package Server;

import java.net.InetAddress;

public class ClientInfo {
    private final InetAddress address;
    private final int port;
    
    public ClientInfo(InetAddress address, int port) {
        this.address = address;
        this.port = port;
    }
    
    public InetAddress getAddress() {
        return address;
    }
    
    public int getPort() {
        return port;
    }
}