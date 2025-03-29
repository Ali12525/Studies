package Client;

import MyPackage.RecIntegral;
import java.util.concurrent.Callable;

public class IntegralTask implements Callable<Double> {
    private RecIntegral integral;
    private double result;

    public IntegralTask(RecIntegral integral) {
        this.integral = integral;
    }
    
    @Override
    public Double call() {
        return calculateIntegral(integral);
    }

    public double getResult() {
        return result;
    }
    
    private double calculateIntegral(RecIntegral integral) {
        double lowLim = integral.getLowLim();
        double upLim = integral.getUpLim();
        double widthLim = integral.getWidthLim();
        boolean isReversed = lowLim > integral.getUpLim();
        
        if (isReversed) {
            double tempBorder = lowLim;
            lowLim = upLim;
            upLim = tempBorder;
        }
        
        double currentLowerBorder = lowLim;    
        long count = (long)((upLim - lowLim) / integral.getWidthLim());
        double sum = 0;
            
        for (long j = 0; j < count; j++) {
            sum += ((widthLim / 2) * (Math.sqrt(currentLowerBorder) + Math.sqrt(currentLowerBorder + widthLim)));
            currentLowerBorder += widthLim;
        }
            
        if((upLim - lowLim) / widthLim > count) {
            currentLowerBorder -= widthLim;
            double lastStepWeigth = upLim - (currentLowerBorder);
            sum += ((lastStepWeigth / 2) * (Math.sqrt(currentLowerBorder) + Math.sqrt(upLim)));
        }
        
        return isReversed ? -sum : sum;
    }
}