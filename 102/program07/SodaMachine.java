/**
 * The SodaMachine class implements a virtual soda machine.
 *
 * Except for testing purposes - DO NOT MODIFY THIS FILE!
 * When graded, your solution will be compiled and run with
 * this class, as written, except that their may be more or
 * fewer soda types and numbers of sodas and the cost of a
 * soda may be different. You should change these things and
 * test that your solution correctly reflects the changes
 * within the bounds specified in the assignment.
 *
 * @author Kurt Mammen
 * @version CPE 102
 */
import java.util.ArrayList;
import java.io.Serializable;


public class SodaMachine implements Serializable
{
   private static final int SODA_COST = 125;
   private ArrayList<Soda> mSodas;
   private int mDeposits;
   private int mSales;

   /**
    * Default constructor
    */
   public SodaMachine()
   {
      mSodas = new ArrayList<Soda>();

      mSodas.add(new Soda("Jolt", 1));
      mSodas.add(new Soda("Squirt", 2));
      mSodas.add(new Soda("Mountain Dew", 3));
      mSodas.add(new Soda("A&W Rootbeer", 4));
      mSodas.add(new Soda("Pepsi", 3));
      mSodas.add(new Soda("Dr. Pepper", 2));
      mSodas.add(new Soda("7-Up", 1));
   }

   /**
    * Dispenses the nth soda if sufficient funds have been deposited.
    * @param index of the soda to dispense
    * @return int The change to return to the customer.
    * @throws InsufficientFundsException when this method is called and not
    * enough money has been deposited to buy a soda.
    * @throws OutOfSodasException when this method is called an there are
    * no sodas to dispense.
    */
   public int dispenseSoda(int index)
   {
      if(!sufficientFunds())
      {
         throw new InsufficientFundsException("Deposits of " + mDeposits + " is less than cost of " + SODA_COST);
      }

      ((Soda)mSodas.get(index)).dispense();

      mSales += SODA_COST;
      int change = mDeposits - SODA_COST;
      mDeposits = 0;

      return change;
   }

   /**
    * Accessor method to determine whether or not there are sufficient funds
    * to purchase a soda.
    * @return boolean true if sufficient funds to purchase a soda are
    * deposited, otherwise false.
    */
   public boolean sufficientFunds()
   {
      if(mDeposits >= SODA_COST)
      {
         return true;
      }

      return false;
   }

   /**
    * Accessor method for the number of DIFFERENT sodas this machine sells.
    * @return int The number of DIFFERENT sodas this machine sells.
    */
   public int getCount()
   {
      return mSodas.size();
   }

   /**
    * Accessor method to get a particular soda's name.
    * @param index identifying the soda whose name you desire.
    * @return String the soda's name.
    */
   public String getSodaName(int index)
   {
      return ((Soda)mSodas.get(index)).getName();
   }

   /**
    * Accessor method to get the number of sodas remaining to be sold.
    * @param index identifying the soda whose count you desire.
    * @return int the number of sodas remaining to be sold.
    */
   public int getSodaCount(int index)
   {
      return ((Soda)mSodas.get(index)).getCount();
   }

   /**
    * Increases the SodaMachine's current deposit amount
    * @param amountInCents to depoist in cents.
    */
   public void deposit(int amountInCents)
   {
      mDeposits += amountInCents;
   }

   /**
    * Accessor method to determine the current amount deposited.
    * @return int the current amount deposited in cents.
    */
   public int getDeposits()
   {
      return mDeposits;
   }

   /**
    * Method to return the current deposits and set deposits to zero.
    * @return int the amount of deposits returned in cents.
    */
   public int returnDeposits()
   {
      int change = mDeposits;
      mDeposits = 0;
      return change;
   }

   /**
    * Accessor method to access the total sales for this SodaMachine.
    * @return int the total sales for this SodaMachine in cents.
    */
   public int getSales()
   {
      return mSales;
   }
}



