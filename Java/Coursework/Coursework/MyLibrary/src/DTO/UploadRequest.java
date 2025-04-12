package DTO;

public class UploadRequest extends RequestDTO {
    private static final long serialVersionUID = 1L;
    private final String destinationPath;
    private final long fileSize; // Размер файла в байтах

    public UploadRequest(String destinationPath, long fileSize) {
        super(CommandType.UPLOAD);
        this.destinationPath = destinationPath;
        this.fileSize = fileSize;
    }

    public String getDestinationPath() {
        return destinationPath;
    }

    public long getFileSize() {
        return fileSize;
    }
}