package lab1;

public class IntegralTask implements Runnable {
    private double lowerBorder;
    private double upperBorder;
    private double weight;
    private double result;

    public IntegralTask(double lowerBorder, double upperBorder, double weight) {
        this.lowerBorder = lowerBorder;
        this.upperBorder = upperBorder;
        this.weight = weight;
    }
    
    @Override
    public void run() {
        result = calculateIntegral(lowerBorder, upperBorder, weight);
    }

    public double getResult() {
        return result;
    }
    
    private double calculateIntegral(double lowerBorder, double upperBorder, double weight) {
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