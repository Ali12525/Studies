package Server;

class StepException extends Exception {
    private final double stepValue;
    
    public StepException(String message, double stepValue) {
        super(message);
        this.stepValue = stepValue;
    }

    public double getStepValue() {
        return stepValue;
    }
}