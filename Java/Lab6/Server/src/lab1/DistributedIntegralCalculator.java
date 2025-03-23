package lab1;

import java.util.concurrent.*;

public class DistributedIntegralCalculator {
    private final double lowerBorder;
    private final double upperBorder;
    private final double weight;
    private final int numberOfThreads;

    public DistributedIntegralCalculator(double lowerBorder, double upperBorder, double weight, int numberOfThreads) {
        this.lowerBorder = lowerBorder;
        this.upperBorder = upperBorder;
        this.weight = weight;
        this.numberOfThreads = numberOfThreads;
    }

    public double calculate() {
        double intervalLength = upperBorder - lowerBorder;
        double subIntervalLength = intervalLength / numberOfThreads;
        double totalIntegral = 0;
        IntegralTask[] tasks = new IntegralTask[numberOfThreads];
        ExecutorService executor = Executors.newFixedThreadPool(numberOfThreads);

        for (int i = 0; i < numberOfThreads; i++) {
            double subLower = lowerBorder + i * subIntervalLength;
            double subUpper = (i == numberOfThreads - 1) ? upperBorder : subLower + subIntervalLength;
            tasks[i] = new IntegralTask(subLower, subUpper, weight);
            executor.execute(tasks[i]);
        }

        executor.shutdown();
        try {
            executor.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        for (IntegralTask task : tasks) {
            totalIntegral += task.getResult();
        }
        return totalIntegral;
    }
}