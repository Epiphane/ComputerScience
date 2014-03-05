/**
 * @author Thomas Steinke
 * @version Lab Quiz 2
 */

public class ThreeNumberLock {
   private int num1, num2, num3;
   private boolean locked;
   
   public ThreeNumberLock(int a, int b, int c) throws IllegalArgumentException {
      if(a < 0 || a > 99 || b < 0 || b > 99 || c < 0 || c > 99) {
         throw new IllegalArgumentException();
      }
      num1 = a;
      num2 = b;
      num3 = c;
      locked = true;
   }
   
   public boolean open(int a, int b, int c) {
      if(a == num1 && b == num2 && c == num3) {
         locked = false;
         return true;
      }
      return false;
   }
   
   public boolean isOpen() {
      return !locked;
   }
   
   public void close() {
      locked = true;
   }
   
   public boolean equals(Object o) {
      if(o == null) {
         return false;
      }
      if(o.getClass() != this.getClass()) {
         return false;
      }
      if(num1 != ((ThreeNumberLock) o).num1) {
         return false;
      }
      if(num2 != ((ThreeNumberLock) o).num2) {
         return false;
      }
      if(num3 != ((ThreeNumberLock) o).num3) {
         return false;
      }
      return true;
   }
   
   public String toString() {
      return "The class is mutable!";
   }
}