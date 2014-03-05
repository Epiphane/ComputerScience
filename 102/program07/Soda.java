/**
 * The Soda class represents a soda by name and count.  This object is used
 * by the SodaMachine class which aggregates them. The count represents the
 * number of sodas of this type.
 *
 * DO NOT MODIFY THIS FILE!
 *
 * @author Kurt Mammen
 * @version CPE 102
 */
import java.io.Serializable;


public class Soda implements Serializable
{
   private String mName;
   private int mCount;

   /**
    * Constructs a Soda object.
    * @param name The name of the soda.
    * @param count The number of sodas.
    */
   public Soda(String name, int count)
   {
      mName = name;
      mCount = count;
   }

   /**
    * Dispenses a soda and reduces the count of sodas remaining
    * @throws OutOfSodasException when this method is called an there are
    * no sodas to dispense.
    */
   public void dispense()
   {
      if(mCount < 1)
      {
         throw new OutOfSodasException("Out of " + mName + "'s");
      }

      mCount--;
   }

   /**
    * Accessor method for the soda's name
    * @return String The name of the soda.
    */
   public String getName()
   {
      return mName;
   }

   /**
    * Accessor method for the number of sodas
    * @return int The number of sodas remaining.
    */
   public int getCount()
   {
      return mCount;
   }
}
