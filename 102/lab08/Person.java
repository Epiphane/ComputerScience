public class Person extends Animal {
   private String name;

   public Person(String name, int legs) {
      super(legs);
      this.name = name;
   }

   public boolean equals(Object other) {
      if(!super.equals(other)) {
         return false;
      }      
      if(name.equals(((Person)other).name)) {
         return false;
      }
      return true;
   }

   public String toString() {
      return super.toString() + " and a Person object whose name is "+name;
   }
}
