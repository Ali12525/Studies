package lab1;

import java.io.Externalizable;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.util.ArrayList;
import java.util.LinkedList;

/**
 * Класс для хранения состояния приложения с использованием интерфейса Externalizable.
 * Сохраняет два списка объектов RecIntegral: основной список и данные для таблицы.
 */
public class SavedStateExtern implements Externalizable {
    private static final long serialVersionUID = 3L;
    private LinkedList<RecIntegral> listRecIntegral;
    private ArrayList<RecIntegral> arrTableData;

    /**
     * Конструктор по умолчанию.
     * Инициализирует пустые списки.
     */
    public SavedStateExtern() {
        this.listRecIntegral = new LinkedList<>();
        this.arrTableData = new ArrayList<>();
    }

    /**
     * Параметризованный конструктор.
     * 
     * @param listRecIntegral список объектов RecIntegral для основного хранения
     * @param arrTableData    список объектов RecIntegral для табличного представления
     */
    public SavedStateExtern(LinkedList<RecIntegral> listRecIntegral, ArrayList<RecIntegral> arrTableData) {
        this.listRecIntegral = new LinkedList<>(listRecIntegral);
        this.arrTableData = new ArrayList<>(arrTableData);
    }

    /**
     * Сериализует объект. Записывает списки в поток вывода.
     * 
     * @param out поток вывода для записи объекта
     * @throws IOException если произошла ошибка ввода-вывода
     */
    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeObject(listRecIntegral);
        out.writeObject(arrTableData);
    }

    /**
     * Десериализует объект. Восстанавливает списки из потока ввода.
     * 
     * @param in поток ввода для чтения объекта
     * @throws IOException            если произошла ошибка ввода-вывода
     * @throws ClassNotFoundException если класс объекта не найден
     */
    @Override
    @SuppressWarnings("unchecked")
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        listRecIntegral = (LinkedList<RecIntegral>) in.readObject();
        arrTableData = (ArrayList<RecIntegral>) in.readObject();
    }

    /**
     * Возвращает копию основного списка объектов RecIntegral.
     * 
     * @return новый LinkedList с объектами RecIntegral
     */
    public LinkedList<RecIntegral> getListRecIntegral() {
        return new LinkedList<>(listRecIntegral);
    }

    /**
     * Возвращает копию списка данных для табличного представления.
     * 
     * @return новый ArrayList с объектами RecIntegral
     */
    public ArrayList<RecIntegral> getArrListTableData() {
        return new ArrayList<>(arrTableData);
    }
    
    @Override
    public String toString() {
        return "SavedStateExtern{" +
                "listRecIntegral=" + listRecIntegral +
                ", arrTableData=" + arrTableData +
                '}';
    }
}