package Server;

import MyPackage.RecIntegral;

public class InputValidator {
    public static RecIntegral validateAndParse(String lower, String upper, String step) throws DataException, StepException {
        
        double lowerVal = parseBoundary(lower);
        double upperVal = parseBoundary(upper);
        double stepVal = parseStep(step);
        
        validateRange(lowerVal, upperVal, stepVal);
        return new RecIntegral(lowerVal, upperVal, stepVal);
    }
    
    private static double parseBoundary(String input) throws DataException {
        try {
            double value = Double.parseDouble(input);
            if (value < 0 || value > 1000000) {
                throw new DataException("Значение границы должно быть от 0 до 1000000");
            }
            return value;
        } catch (NumberFormatException e) {
            throw new DataException("Некорректный числовой формат для границы");
        }
    }
    
    private static double parseStep(String input) throws DataException, StepException {
        try {
            double value = Double.parseDouble(input);
            if (value <= 0 || value > 1000000) {
                throw new StepException("Значение шага должно быть больше 0 и до 1000000", value);
            }
            return value;
        } catch (NumberFormatException e) {
            throw new DataException("Некорректный числовой формат для шага");
        }
    }

    private static void validateRange(double lower, double upper, double step) throws DataException {
        if (Math.abs(upper - lower) < step) {
            throw new DataException("Интервал должен быть не меньше шага");
        }
    }
}
