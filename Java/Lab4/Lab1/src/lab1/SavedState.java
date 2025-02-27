/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package lab1;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.LinkedList;

/**
 *
 * @author student
 */
public class SavedState implements Serializable {
    private static final long serialVersionUID = 2L;
    private LinkedList<RecIntegral> listRecIntegral = new LinkedList<>();
    private ArrayList<RecIntegral> listTableData = new ArrayList<>();
    
    public SavedState(LinkedList<RecIntegral> listRecIntegral, ArrayList<RecIntegral> listTableData) {
        this.listRecIntegral = new LinkedList<>(listRecIntegral);
        this.listTableData = new ArrayList<>(listTableData);
    }
    
    public LinkedList<RecIntegral> getListRecIntegral() {
        return new LinkedList<>(listRecIntegral);
    }

    public ArrayList<RecIntegral> getListTableData() {
        return new ArrayList<>(listTableData);
    }
}
