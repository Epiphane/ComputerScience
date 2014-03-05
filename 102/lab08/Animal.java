public class Animal {
   private int legs;

   Animal(int legs) {
      this.legs = legs;
   }

   public boolean equals(Object other) {
      if(other == null) {
         return false;
      }
      if(other.getClass() != this.getClass()) {
         return false;
      }
      if(legs != ((Animal)other).legs) {
         return false;
      }
      return true;
   }

   public String toString() {
      return "I am an Animal object with "+legs+" legs";
   }
}
