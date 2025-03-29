package Client;

import java.util.concurrent.*;
import MyPackage.RecIntegral;
import java.util.ArrayList;
import java.util.List;

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
        ExecutorService executor = Executors.newFixedThreadPool(numberOfThreads);
        List<Future<Double>> futures = new ArrayList<>();

        for (int i = 0; i < numberOfThreads; i++) {
            double subLower = lowLim + i * subIntervalLength;
            double subUpper = (i == numberOfThreads - 1) ? upLim : subLower + subIntervalLength;
            Callable<Double> task = new IntegralTask(new RecIntegral(subLower, subUpper, integral.getWidthLim()));
            futures.add(executor.submit(task));
        }

        executor.shutdown();
        
        for (Future<Double> future : futures) {
            try {
                totalIntegral += future.get();
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
        }
        return totalIntegral;
    }
}