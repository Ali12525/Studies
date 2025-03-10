package lab1;

public class IntegralCalculator {
    public static double calculate(double lowerBorder, double upperBorder, double weight) {
        boolean isReversed = lowerBorder > upperBorder;
        
        if (isReversed) {
            double tempBorder = lowerBorder;
            lowerBorder = upperBorder;
            upperBorder = tempBorder;
        }
        
        double currentLowerBorder = lowerBorder;    
        long count = (long)((upperBorder - lowerBorder) / weight);
        double sum = 0;
            
        for (long j = 0; j < count; j++) {
            sum += ((weight / 2) * (Math.sqrt(currentLowerBorder) + Math.sqrt(currentLowerBorder + weight)));
            currentLowerBorder += weight;
        }
            
        if((upperBorder - lowerBorder) / weight > count) {
            currentLowerBorder -= weight;
            double lastStepWeigth = upperBorder - (currentLowerBorder);
            sum += ((lastStepWeigth / 2) * (Math.sqrt(currentLowerBorder) + Math.sqrt(upperBorder)));
        }
        
        return isReversed ? -sum : sum;
    }
}