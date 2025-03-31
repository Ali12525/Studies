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
    
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof ClientInfo)) return false;
        ClientInfo other = (ClientInfo) obj;
        return address.equals(other.address) && port == other.port;
    }
    
    @Override
    public int hashCode() {
        return address.hashCode() + port;
    }
}