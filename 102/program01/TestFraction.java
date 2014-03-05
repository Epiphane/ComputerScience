/**
  * Test Fraction class - a driver to test my Fraction class
  *
  * @author Thomas Steinke
  * @version Program 1
  */
			  
public class TestFraction {
   public static void main(String[] args) {
      testConstructors();
      testAccessors();
      testAdd();
      testSub();
      testMul();
      testDiv();
      testEquals();
      testValue();
   }
   
   public static void testConstructors() {
      Fraction f1 = new Fraction();
      Fraction f2 = new Fraction(2);
      try {
         Fraction f3 = new Fraction(3,-7);
      }
      catch(IllegalArgumentException e) {
         System.out.println("You dummy! The denominator can't be negative! It says in the prompt!");
      }
      
      Fraction f4 = new Fraction(3,7);
      
      if(!f1.toString().equals("0")) {
         System.out.println("Error with Fraction(): Expected 0, got "+f1.toString());
      }
      if(!f2.toString().equals("2")) {
         System.out.println("Error with Fraction(2): Expected 2, got "+f2.toString());
      }
      if(!f4.toString().equals("3/7")) {
         System.out.println("Error with Fraction(3,7): Expected 3/7, got "+f4.toString());
      }
   }
   
   public static void testAccessors() {
      Fraction f1 = new Fraction(3, 7);
      
      if(f1.getNumerator() != 3) {
         System.out.println("Error getNumerator(): Expected 3, got "+f1.getNumerator());
      }
      if(f1.getDenominator() != 7) {
         System.out.println("Error getDenominator(): Expected 7, got "+f1.getDenominator());
      }
   }
   
   public static void testAdd() {
      Fraction f1 = new Fraction(1,4);
      Fraction f2 = new Fraction(3,10);
      
      Fraction f3 = f1.add(f2);
      if(!f3.equals(new Fraction(11,20))) {
         System.out.println("Error with add(): Expected 11/20, got "+f3.toString());
      }
   }
   
   public static void testSub() {
      Fraction f1 = new Fraction(1,4);
      Fraction f2 = new Fraction(3,10);
      
      Fraction f3 = f1.sub(f2);
      if(!f3.equals(new Fraction(-1,20))) {
         System.out.println("Error with sub(): Expected -1/20, got "+f3.toString());
      }
   }
   
   public static void testMul() {
      Fraction f1 = new Fraction(3,4);
      Fraction f2 = new Fraction(4,15);
      
      Fraction f3 = f1.mul(f2);
      if(!f3.equals(new Fraction(1,5))) {
         System.out.println("Error with mul(): Expected 1/5, got "+f3.toString());
      }
   }
   
   public static void testDiv() {
      Fraction f1 = new Fraction(1,4);
      Fraction f2 = new Fraction(5,12);
      
      Fraction f3 = f1.div(f2);
      if(!f3.equals(new Fraction(3,5))) {
         System.out.println("Error with div(): Expected 3/5, got "+f3.toString());
      }
   }
   
   public static void testEquals() {
      Fraction f1 = new Fraction(3,7);
      Fraction f2 = new Fraction(3,7);
      if(!f1.equals(f2)) {
         System.out.println("Error with equals(): Expected equality");
      }
      
      Fraction f3 = new Fraction(7,11);
      if(f1.equals(f3)) {
         System.out.println("Error with equals(): Expected inequality");
      }
   }
   
   public static void testValue() {
      Fraction f1 = new Fraction(1,32);
      if(f1.value() != 0.03125) {
         System.out.println("Error with value(): Expected 0.03125, got "+f1.value());
      }
   }
}