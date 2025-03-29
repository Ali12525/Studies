package MyPackage;

import java.io.Serializable;

/**
 * Класс для хранения данных о вычислении интеграла.
 * Реализует интерфейс Serializable для поддержки сериализации.
 */
public class RecIntegral implements Serializable {
    private static final long serialVersionUID = 1L;

    private double widthLim;
    private double lowLim;
    private double upLim;
    private double resIntegral;
    
     /**
     * Конструктор с необязательным resIntegral (по умолчанию Double.MAX_VALUE)
     */
    public RecIntegral(double lowLim, double upLim, double widthLim) {
        this(lowLim, upLim, widthLim, Double.MAX_VALUE);
    }

    /**
     * Конструктор для создания объекта RecIntegral.
     *
     * @param lowLim      Нижняя граница.
     * @param upLim       Верхняя граница.
     * @param widthLim    Ширина шага.
     * @param resIntegral Результат вычисления интеграла.
     */
    public RecIntegral(double lowLim, double upLim, double widthLim, double resIntegral) {
        this.widthLim = widthLim;
        this.lowLim = lowLim;
        this.upLim = upLim;
        this.resIntegral = resIntegral;
    }

    /**
     * Возвращает нижнюю границу.
     *
     * @return Нижняя граница.
     */
    public double getLowLim() {
        return lowLim;
    }

    /**
     * Устанавливает нижнюю границу.
     *
     * @param lowLim Нижняя граница.
     */
    public void setLowLim(double lowLim) {
        this.lowLim = lowLim;
    }

    /**
     * Возвращает верхнюю границу.
     *
     * @return Верхняя граница.
     */
    public double getUpLim() {
        return upLim;
    }

    /**
     * Устанавливает верхнюю границу.
     *
     * @param upLim Верхняя граница.
     */
    public void setUpLim(double upLim) {
        this.upLim = upLim;
    }

    /**
     * Возвращает ширину шага.
     *
     * @return Ширина шага.
     */
    public double getWidthLim() {
        return widthLim;
    }

    /**
     * Устанавливает ширину шага.
     *
     * @param widthLim Ширина шага.
     */
    public void setWidthLim(double widthLim) {
        this.widthLim = widthLim;
    }

    /**
     * Возвращает результат вычисления интеграла.
     *
     * @return Результат вычисления интеграла.
     */
    public double getResIntegral() {
        return resIntegral;
    }

    /**
     * Устанавливает результат вычисления интеграла.
     *
     * @param resIntegral Результат вычисления интеграла.
     */
    public void setResIntegral(double resIntegral) {
        this.resIntegral = resIntegral;
    }

    @Override
    public String toString() {
        return "RecIntegral{" +
                "lowLim=" + lowLim +
                ", upLim=" + upLim +
                ", widthLim=" + widthLim +
                ", resIntegral=" + resIntegral +
                '}';
    }
}