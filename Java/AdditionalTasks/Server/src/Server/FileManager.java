package Server;

import javax.swing.JFrame;
import java.io.*;
import java.util.ArrayList;
import java.util.LinkedList;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import MyPackage.RecIntegral;

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
    
    // JSON File Operations
    public void saveToJsonFile(File file, SavedState data) {
        try (FileWriter writer = new FileWriter(file)) {
            StringBuilder sb = new StringBuilder();
            sb.append("{\n");
            
            sb.append("  \"listRecIntegral\": [\n");
            boolean first = true;
            for (RecIntegral rec : data.getListRecIntegral()) {
                if (!first) {
                    sb.append(",\n");
                } else {
                    first = false;
                }
                sb.append("    {");
                sb.append("\"lowLim\": ").append(rec.getLowLim()).append(", ");
                sb.append("\"upLim\": ").append(rec.getUpLim()).append(", ");
                sb.append("\"widthLim\": ").append(rec.getWidthLim()).append(", ");
                sb.append("\"resIntegral\": ").append(rec.getResIntegral());
                sb.append("}");
            }
            sb.append("\n  ],\n");
            
            sb.append("  \"arrListTableData\": [\n");
            first = true;
            for (RecIntegral rec : data.getArrListTableData()) {
                if (!first) {
                    sb.append(",\n");
                } else {
                    first = false;
                }
                sb.append("    {");
                sb.append("\"lowLim\": ").append(rec.getLowLim()).append(", ");
                sb.append("\"upLim\": ").append(rec.getUpLim()).append(", ");
                sb.append("\"widthLim\": ").append(rec.getWidthLim()).append(", ");
                sb.append("\"resIntegral\": ").append(rec.getResIntegral());
                sb.append("}");
            }
            sb.append("\n  ]\n");
            sb.append("}\n");
            
            writer.write(sb.toString());
        } catch (IOException e) {
            JOptionPane.showMessageDialog(parentFrame, "Ошибка при сохранении JSON файла: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    public SavedState loadFromJsonFile(File file) {
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                sb.append(line);
            }
            String json = sb.toString();
            
            LinkedList<RecIntegral> listRecIntegral = new LinkedList<>();
            ArrayList<RecIntegral> arrTableData = new ArrayList<>();
            
            int start = json.indexOf("\"listRecIntegral\":");
            if (start != -1) {
                start = json.indexOf("[", start);
                int end = json.indexOf("]", start);
                if (start != -1 && end != -1) {
                    String listArray = json.substring(start + 1, end);
                    String[] recStrings = listArray.split("},");
                    for (String recStr : recStrings) {
                        recStr = recStr.trim();
                        if (!recStr.endsWith("}")) {
                            recStr += "}";
                        }
                        RecIntegral rec = parseRecIntegral(recStr);
                        listRecIntegral.add(rec);
                    }
                }
            }
            
            start = json.indexOf("\"arrListTableData\":");
            if (start != -1) {
                start = json.indexOf("[", start);
                int end = json.indexOf("]", start);
                if (start != -1 && end != -1) {
                    String arrArray = json.substring(start + 1, end);
                    String[] recStrings = arrArray.split("},");
                    for (String recStr : recStrings) {
                        recStr = recStr.trim();
                        if (!recStr.endsWith("}")) {
                            recStr += "}";
                        }
                        RecIntegral rec = parseRecIntegral(recStr);
                        arrTableData.add(rec);
                    }
                }
            }
            return new SavedState(listRecIntegral, arrTableData);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(parentFrame, "Ошибка при загрузке JSON файла: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
            return null;
        }
    }
    
    private RecIntegral parseRecIntegral(String recStr) {
        recStr = recStr.replace("{", "").replace("}", "");
        String[] parts = recStr.split(",");
        double lowLim = 0, upLim = 0, widthLim = 0, resIntegral = 0;
        for (String part : parts) {
            part = part.trim();
            String[] keyValue = part.split(":");
            if (keyValue.length == 2) {
                String key = keyValue[0].replace("\"", "").trim();
                String value = keyValue[1].trim();
                switch (key) {
                    case "lowLim":
                        lowLim = Double.parseDouble(value);
                        break;
                    case "upLim":
                        upLim = Double.parseDouble(value);
                        break;
                    case "widthLim":
                        widthLim = Double.parseDouble(value);
                        break;
                    case "resIntegral":
                        resIntegral = Double.parseDouble(value);
                        break;
                }
            }
        }
        return new RecIntegral(lowLim, upLim, widthLim, resIntegral);
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
    
    public File getPathJsonFileToSaved() {
        return getFilePath(JFileChooser.SAVE_DIALOG, ".json", "JSON Files (*.json)");
    }
    
    public File getPathJsonFileToLoad() {
        return getFilePath(JFileChooser.OPEN_DIALOG, ".json", "JSON Files (*.json)");
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