/**
 * @author Kurt Mammen
 * @version Soda Machine
 *
 * Exception thrown if you try to dispense a soda with insufficient funds.
 */

public class InsufficientFundsException extends RuntimeException
{
   /**
    * Constructor
    * @param msg The exception message.
    */
   public InsufficientFundsException(String msg)
   {
      super(msg);
   }
}