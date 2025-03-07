package lab1;

import javax.swing.JFrame;
import java.io.*;
import java.util.ArrayList;
import java.util.LinkedList;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

public class FileManager {
    private final JFrame parentFrame;

    public FileManager(JFrame parentFrame) {
        this.parentFrame = parentFrame;
    }
    
    //Text File Operations
    public void saveToTextFile(File file, SavedState state) {
        try (FileWriter writer = new FileWriter(file)) {
            writeRecIntegralList(writer, state.getListRecIntegral());
            writer.write("---\n");
            writeRecIntegralList(writer, state.getArrListTableData());
        } catch (IOException e) {
            JOptionPane.showMessageDialog(parentFrame, "Ошибка при сохранении файла: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    public SavedState loadFromTextFile(File file) {
        LinkedList<RecIntegral> listRecIntegral = new LinkedList<>();
        ArrayList<RecIntegral> arrTableData = new ArrayList<>();

        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            parseTextFile(reader, listRecIntegral, arrTableData);
            return new SavedState(listRecIntegral, arrTableData);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(parentFrame, "Ошибка при загрузке файла: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
            return null;
        }
    }
    
    //Serializable File Operations
    public void saveToBinaryFile(File file, SavedState data) {
    try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(file))) {
        oos.writeObject(data);
    } catch (IOException e) {
        JOptionPane.showMessageDialog(parentFrame, "Ошибка при сохранении файла: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    public SavedState loadFromBinaryFile(File file) {
    try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file))) {
        return (SavedState) ois.readObject();
    } catch (IOException | ClassNotFoundException e) {
        JOptionPane.showMessageDialog(parentFrame, "Ошибка при загрузке файла: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        return null;
        }
    }
    
    //Externalizable File Operations
    public void saveToBinaryExternFile(File file, SavedStateExtern data) {
    try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(file))) {
        oos.writeObject(data);
    } catch (IOException e) {
        JOptionPane.showMessageDialog(parentFrame, "Ошибка при сохранении файла: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    public SavedStateExtern loadFromBinaryExternFile(File file) {
    try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file))) {
        return (SavedStateExtern) ois.readObject();
    } catch (IOException | ClassNotFoundException e) {
        JOptionPane.showMessageDialog(parentFrame, "Ошибка при загрузке файла: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        return null;
        }
    }
    
    //File Dialogs
    private File getFilePath(int mode, String extension, String description) {
        JFileChooser fileChooser = new JFileChooser();
        FileNameExtensionFilter filter = new FileNameExtensionFilter(description, extension.substring(1));
        fileChooser.setFileFilter(filter);

        while (true) {
            int option = (mode == JFileChooser.SAVE_DIALOG) ? fileChooser.showSaveDialog(null) : fileChooser.showOpenDialog(null);

            if (option == JFileChooser.APPROVE_OPTION) {
                File file = fileChooser.getSelectedFile();
                if (file == null) return null;
                if (!file.getName().contains(".")) {
                    file = new File(file.getAbsolutePath() + extension);
                }
                else if (!file.getName().toLowerCase().endsWith(extension)) {
                    JOptionPane.showMessageDialog(parentFrame, "Файл должен иметь расширение " + extension + ". Пожалуйста, выберите другой файл.", "Ошибка", JOptionPane.ERROR_MESSAGE );
                    continue;
                }

                if (mode == JFileChooser.SAVE_DIALOG && file.exists()) {
                    int overwriteOption = JOptionPane.showConfirmDialog(parentFrame, "Файл уже существует. Перезаписать?", "Предупреждение", JOptionPane.YES_NO_OPTION);
                    if (overwriteOption != JOptionPane.YES_OPTION) {
                        return null;
                    }
                }

                if (mode == JFileChooser.OPEN_DIALOG && !file.exists()) {
                    JOptionPane.showMessageDialog(parentFrame, "Файл не существует.", "Ошибка", JOptionPane.ERROR_MESSAGE);
                    return null;
                }

                return file;
            } else {
                return null;
            }
        }
    }
    
    public File getPathSerFileToSaved() {
        return getFilePath(JFileChooser.SAVE_DIALOG, ".ser", "Serialized Files (*.ser)");
    }

    public File getPathSerFileToLoad() {
        return getFilePath(JFileChooser.OPEN_DIALOG, ".ser", "Serialized Files (*.ser)");
    }

    public File getPathTXTFileToSaved() {
        return getFilePath(JFileChooser.SAVE_DIALOG, ".txt", "Text Files (*.txt)");
    }

    public File getPathTXTFileToLoad() {
        return getFilePath(JFileChooser.OPEN_DIALOG, ".txt", "Text Files (*.txt)");
    }
    
    public File getPathExternFileToSaved() {
        return getFilePath(JFileChooser.SAVE_DIALOG, ".dat", "Externalizable Files (*.dat)");
    }

    public File getPathExternFileToLoad() {
        return getFilePath(JFileChooser.OPEN_DIALOG, ".dat", "Externalizable Files (*.dat)");
    }
    
    //Private Helpers
    private void writeRecIntegralList(FileWriter writer, Iterable<RecIntegral> list) throws IOException {
        for (RecIntegral recIntegral : list) {
            writer.write(String.format("%f;%f;%f;%f%n",
                recIntegral.getLowLim(),
                recIntegral.getUpLim(),
                recIntegral.getWidthLim(),
                recIntegral.getResIntegral()));
        }
    }
    
    private void parseTextFile(BufferedReader reader, LinkedList<RecIntegral> listRecIntegral, ArrayList<RecIntegral> arrTableData) throws IOException {
        String line;
        boolean isListRecIntegral = true;

        while ((line = reader.readLine()) != null) {
            if (line.equals("---")) {
                isListRecIntegral = false;
                continue;
            }

            String[] parts = line.split(";");
            if (parts.length == 4) {
                RecIntegral recIntegral = createRecIntegralFromParts(parts);
                if (isListRecIntegral) {
                    listRecIntegral.add(recIntegral);
                } else {
                    arrTableData.add(recIntegral);
                }
            }
        }
    }
    
    private RecIntegral createRecIntegralFromParts(String[] parts) {
        String lowLimStr = parts[0].replace(',', '.');
        String upLimStr = parts[1].replace(',', '.');
        String widthLimStr = parts[2].replace(',', '.');
        String resIntegralStr = parts[3].replace(',', '.');
        
        double lowLim = Double.parseDouble(lowLimStr);
        double upLim = Double.parseDouble(upLimStr);
        double widthLim = Double.parseDouble(widthLimStr);
        double resIntegral = Double.parseDouble(resIntegralStr);
        return new RecIntegral(lowLim, upLim, widthLim, resIntegral);
    }
}