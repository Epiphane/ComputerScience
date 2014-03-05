public class TestDriver {
   public static void main(String[] args) {
      boolean pass = true;
   
      pass &= testThreeNumberLock();
      pass &= testFourNumberLock();
      
      if(pass) System.out.println("Yay.");
   }
   
   public static boolean testThreeNumberLock() {
      boolean pass = true;
   
      int test = 1;
      ThreeNumberLock l1 = new ThreeNumberLock(1, 2, 3);
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, !l1.open(1, 2, 4), "Lock shouldn't open!");
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, !l1.open(1, 3, 2), "Lock shouldn't open!");
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, !l1.open(2, 2, 3), "Lock shouldn't open!");
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, l1.open(1, 2, 3), "Lock should open!");
      pass &= test(test++, l1.isOpen(), "Lock should be open!");
      
      l1.close();
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, l1.equals(l1), "Lock should equal itself!");
      
      ThreeNumberLock l2 = new ThreeNumberLock(1, 2, 3);
      ThreeNumberLock l3 = new ThreeNumberLock(1, 2, 4);
      l1.close();
      pass &= test(test++, l1.equals(l2), "Locks should be equal!");
      pass &= test(test++, !l1.equals(l3), "Locks should not be equal!");
      l1.open(1, 2, 3);
      pass &= test(test++, l1.equals(l2), "Locks should be equal!");
      pass &= test(test++, !l1.equals(l3), "Locks should not be equal!");
      
      ThreeNumberLock l4 = l1;
      l4.close();
      
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      
      return pass;
   }
   
   public static boolean testFourNumberLock() {
      boolean pass = true;
      
      int test = 1;
      FourNumberLock l1 = new FourNumberLock(1, 2, 3, 4);
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, !l1.open(1, 2, 3), "Lock shouldn't be able to open!");
      pass &= test(test++, !l1.open(1, 2, 3, 5), "Lock shouldn't open!");
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, !l1.open(1, 2, 4, 4), "Lock shouldn't open!");
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, !l1.open(1, 3, 3, 4), "Lock shouldn't open!");
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, !l1.open(2, 2, 3, 4), "Lock shouldn't open!");
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, l1.open(1, 2, 3, 4), "Lock should open!");
      pass &= test(test++, l1.isOpen(), "Lock should be open!");
      
      l1.close();
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      pass &= test(test++, l1.equals(l1), "Lock should equal itself!");
      
      FourNumberLock l2 = new FourNumberLock(1, 2, 3, 4);
      FourNumberLock l3 = new FourNumberLock(1, 2, 4, 5);
      FourNumberLock l4 = new FourNumberLock(1, 2, 3, 5);
      l1.close();
      pass &= test(test++, l1.equals(l2), "Locks should be equal!");
      pass &= test(test++, !l1.equals(l3), "Locks should not be equal!");
      pass &= test(test++, !l1.equals(l4), "Locks should not be equal!");
      l1.open(1, 2, 3);
      pass &= test(test++, l1.equals(l2), "Locks should be equal!");
      pass &= test(test++, !l1.equals(l3), "Locks should not be equal!");
      pass &= test(test++, !l1.equals(l4), "Locks should not be equal!");
      
      FourNumberLock l5 = l1;
      l5.close();
      
      pass &= test(test++, !l1.isOpen(), "Lock should be closed!");
      
      return pass;
   }
   
   public static boolean test(int test, boolean result, String message) {
      if(!result) {
         System.out.println("FAILED test "+test+": "+message);
      }
      
      return result;
   }
}