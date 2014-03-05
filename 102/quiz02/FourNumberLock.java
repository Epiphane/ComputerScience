/**
 * @author Thomas Steinke
 * @version Lab Quiz 2
 */

public class FourNumberLock extends ThreeNumberLock {
   private int num4;
   
   public FourNumberLock(int a, int b, int c, int d) throws IllegalArgumentException {
      super(a, b, c);
      if(d < 0 || d > 99) {
         throw new IllegalArgumentException();
      }
      num4 = d;
   }
   
   public boolean open(int a, int b, int c) {
      return false;
   }
   
   public boolean open(int a, int b, int c, int d) {
      if(d != num4) {
         return false;
      }
      return super.open(a, b, c);
   }
   
   public boolean equals(Object o) {
      if(!super.equals(o)) {
         return false;
      }
      if(num4 != ((FourNumberLock) o).num4) {
         return false;
      }
      return true;
   }
}