package lab1;

/**
 * Исключение, которое выбрасывается при некорректных данных.
 * Наследуется от класса Exception.
 */
public class DataException extends Exception {
    
     /**
     * Создает новое исключение с указанным сообщением.
     *
     * @param message Сообщение об ошибке.
     */
    public DataException (String message){
        super(message);
    }
}
