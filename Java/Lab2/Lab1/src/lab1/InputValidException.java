/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package lab1;

/**
 *
 * @author milad
 */
public class InputValidException extends Exception {
    private double rangeWidthLowLim;
    private double rangeWidthUpLim;
    private double widthUpLim;
    
    public InputValidException(double rangeWidthLowLim, double rangeWidthUpLim, double widthUpLim){
        this.rangeWidthLowLim = rangeWidthLowLim;
        this.rangeWidthUpLim = rangeWidthUpLim;
        this.widthUpLim = widthUpLim;
    }
    
    public InputValidException(String message){
        super(message);
    }
}
