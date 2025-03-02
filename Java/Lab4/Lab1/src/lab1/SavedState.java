package lab1;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.LinkedList;

/**
 * Класс для хранения состояния приложения, включающего два списка объектов RecIntegral.
 * Реализует интерфейс Serializable для поддержки сериализации.
 */
public class SavedState implements Serializable {
    private static final long serialVersionUID = 2L;
    private LinkedList<RecIntegral> listRecIntegral = new LinkedList<>();
    private ArrayList<RecIntegral> arrTableData = new ArrayList<>();
    
    /**
     * Конструктор для создания объекта SavedState.
     *
     * @param listRecIntegral Список RecIntegral для хранения.
     * @param arrTableData    Список данных таблицы для хранения.
     */
    public SavedState(LinkedList<RecIntegral> listRecIntegral, ArrayList<RecIntegral> arrTableData) {
        this.listRecIntegral = new LinkedList<>(listRecIntegral);
        this.arrTableData = new ArrayList<>(arrTableData);
    }
    
    /**
     * Возвращает копию списка RecIntegral вне таблицы.
     *
     * @return Копия списка RecIntegral вне таблицы.
     */
    public LinkedList<RecIntegral> getListRecIntegral() {
        return new LinkedList<>(listRecIntegral);
    }

     /**
     * Возвращает копию списка данных таблицы.
     *
     * @return Копия списка данных таблицы.
     */
    public ArrayList<RecIntegral> getArrListTableData() {
        return new ArrayList<>(arrTableData);
    }
    
    @Override
    public String toString() {
        return "SavedState{" +
                "listRecIntegral=" + listRecIntegral +
                ", arrTableData=" + arrTableData +
                '}';
    }
}