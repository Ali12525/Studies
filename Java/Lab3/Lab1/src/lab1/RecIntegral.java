/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package lab1;

/**
 *
 * @author student
 */
public class RecIntegral {
    private double widthLim, lowLim, upLim, resIntegral;
    
    public RecIntegral(double lowLim, double upLim, double widthLim, double resIntegral) {
        this.widthLim = widthLim;
        this.lowLim = lowLim;
        this.upLim = upLim;
        this.resIntegral = resIntegral;
    }
    
    public double getLowLim() {
        return lowLim;
    }
    
    public double getUpLim() {
        return upLim;
    }
    
    public double getWidthLim() {
        return widthLim;
    }
    
    public double getResIntegral() {
        return resIntegral;
    }
    
    public void setLowLim(double LowLim) {
        this.lowLim = lowLim;
    }
    
    public void setUpLim(double upLim) {
        this.upLim = upLim;
    }
    
    public void setWidthLim(double widthLim) {
        this.widthLim = widthLim;
    }
    
    public void setResIntegral(double resIntegral) {
        this.resIntegral = resIntegral;
    }
}