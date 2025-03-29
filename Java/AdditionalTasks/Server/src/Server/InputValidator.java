package Server;

import MyPackage.RecIntegral;

public class InputValidator {
    public static RecIntegral validateAndParse(String lower, String upper, String step) throws DataException {
        
        double lowerVal = parseValue(lower);
        double upperVal = parseValue(upper);
        double stepVal = parseValue(step);
        
        validateRange(lowerVal, upperVal, stepVal);
        return new RecIntegral(lowerVal, upperVal, stepVal);
    }
    
    private static double parseValue(String input) throws DataException {
        try {
            double value = Double.parseDouble(input);
            if (value == 0 || (value >= 0.01 && value <= 1)) {
                throw new DataException("Значение должно быть от 0.000001 до 1000000");
            }
            return value;
        } catch (NumberFormatException e) {
            throw new DataException("Некорректный числовой формат");
        }
    }

    private static void validateRange(double lower, double upper, double step) throws DataException {
        
        if (Math.abs(upper - lower) < step) {
            throw new DataException("Интервал должен быть не меньше шага");
        }
    }
}