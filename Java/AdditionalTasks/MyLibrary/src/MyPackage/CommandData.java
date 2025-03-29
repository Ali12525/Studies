package MyPackage;

import java.io.Serializable;

public class CommandData implements Serializable {
    private static final long serialVersionUID = 4L;

    private String commandType;
    private RecIntegral integral;
    
    public CommandData(String commandType, RecIntegral integral) {
        this.commandType = commandType;
        this.integral = integral;
    }
    
    public CommandData(String commandType, double resIntegral) {
        this(commandType, new RecIntegral(0, 0, 0, resIntegral));
    }

    public String getCommandType() { return commandType; }
    public double getResIntegral() { return integral.getResIntegral();}
    public RecIntegral getRecIntegral() { return integral; }

    @Override
    public String toString() {
        if (integral == null) {
            return "CommandData{" +
                    "commandType='" + commandType + '\'' +
                    ", integral=null" +
                    '}';
        }
        return "CommandData{" +
                "commandType='" + commandType + '\'' +
                ", lowLim=" + integral.getLowLim() +
                ", upLim=" + integral.getUpLim() +
                ", widthLim=" + integral.getWidthLim() +
                ", resIntegral=" + integral.getResIntegral() +
                '}';
    }
}