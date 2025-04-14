package DTO;

import java.io.Serializable;

public class FileInfo implements Serializable {
    private static final long serialVersionUID = 1L;
    private final String name;
    private final long size;
    private final long lastModified;

    public FileInfo(String name, long size, long lastModified) {
        this.name = name;
        this.size = size;
        this.lastModified = lastModified;
    }

    public String getName() {
        return name;
    }

    public long getSize() {
        return size;
    }

    public long getLastModified() {
        return lastModified;
    }
}