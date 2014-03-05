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
      Fraction f3 = new Fraction(3,7);
      
      if(!f1.toString().equals("0/1")) {
         System.out.println("Error with Fraction(): Expected 0/1, got "+f1.toString());
      }
      if(!f2.toString().equals("2/1")) {
         System.out.println("Error with Fraction(2): Expected 2/1, got "+f2.toString());
      }
      if(!f3.toString().equals("3/7")) {
         System.out.println("Error with Fraction(3,7): Expected 3/7, got "+f3.toString());
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
      Fraction f1 = new Fraction(3,7);
      Fraction f2 = new Fraction(7,11);
      
      Fraction f3 = f1.add(f2);
      if(!f3.equals(new Fraction(82,77))) {
         System.out.println("Error with add(): Expected 82/77, got "+f3.toString());
      }
   }
   
   public static void testSub() {
      Fraction f1 = new Fraction(7,11);
      Fraction f2 = new Fraction(3,7);
      
      Fraction f3 = f1.sub(f2);
      if(!f3.equals(new Fraction(16,77))) {
         System.out.println("Error with sub(): Expected 16/77, got "+f3.toString());
      }
   }
   
   public static void testMul() {
      Fraction f1 = new Fraction(7,11);
      Fraction f2 = new Fraction(3,7);
      
      Fraction f3 = f1.mul(f2);
      if(!f3.equals(new Fraction(21,77))) {
         System.out.println("Error with mul(): Expected 21/77, got "+f3.toString());
      }
   }
   
   public static void testDiv() {
      Fraction f1 = new Fraction(3,7);
      Fraction f2 = new Fraction(7,11);
      
      Fraction f3 = f1.div(f2);
      if(!f3.equals(new Fraction(33,49))) {
         System.out.println("Error with div(): Expected 33/49, got "+f3.toString());
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
         System.out.println("Error with getValue(): Expected 0.03125, got "+f1.value());
      }
   }
}