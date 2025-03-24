package MyPackage;

import java.io.Serializable;

public class CommandData implements Serializable {
    private static final long serialVersionUID = 4L;

    private String commandType;
    private double lowLim;
    private double upLim;
    private double widthLim;
    private double resIntegral;

    public CommandData(String commandType, double lowLim, double upLim, double widthLim) {
        this.commandType = commandType;
        this.lowLim = lowLim;
        this.upLim = upLim;
        this.widthLim = widthLim;
    }
    
    public CommandData(String commandType, double resIntegral) {
        this.commandType = commandType;
        this.resIntegral = resIntegral;
    }

    public String getCommandType() { return commandType; }
    public double getLowLim() { return lowLim; }
    public double getUpLim() { return upLim; }
    public double getWidthLim() { return widthLim; }
    public double getResIntegral() { return resIntegral; }

    @Override
    public String toString() {
        return "CommandData{" +
                "commandType='" + commandType + '\'' +
                ", lowLim=" + lowLim +
                ", upLim=" + upLim +
                ", widthLim=" + widthLim +
                ", resIntegral=" + resIntegral +
                '}';
    }
}