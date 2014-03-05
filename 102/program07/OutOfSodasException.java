/**
 * @author Kurt Mammen
 * @version Soda Machine
 *
 * Exception thrown if you try to dispense a soda and there aren't any.
 */

public class OutOfSodasException extends RuntimeException
{
   /**
    * Constructor
    * @param msg The exception message.
    */
   public OutOfSodasException(String msg)
   {
      super(msg);
   }
}