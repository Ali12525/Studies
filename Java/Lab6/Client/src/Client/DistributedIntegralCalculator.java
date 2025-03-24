package Client;

import java.util.concurrent.*;
import MyPackage.RecIntegral;

public class DistributedIntegralCalculator {
    private final RecIntegral integral;
    private final int numberOfThreads;

    public DistributedIntegralCalculator(RecIntegral integral, int numberOfThreads) {
        this.integral = integral;
        this.numberOfThreads = numberOfThreads;
    }

    public double calculate() {
        double lowLim = integral.getLowLim();
        double upLim = integral.getUpLim();
        double intervalLength = upLim - lowLim;
        double subIntervalLength = intervalLength / numberOfThreads;
        double totalIntegral = 0;
        IntegralTask[] tasks = new IntegralTask[numberOfThreads];
        ExecutorService executor = Executors.newFixedThreadPool(numberOfThreads);

        for (int i = 0; i < numberOfThreads; i++) {
            double subLower = lowLim + i * subIntervalLength;
            double subUpper = (i == numberOfThreads - 1) ? upLim : subLower + subIntervalLength;
            tasks[i] = new IntegralTask(new RecIntegral(subLower, subUpper, integral.getWidthLim()));
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