public class Greeter {
   private String myName;

   public Greeter(String name) {
      // Assign myName to name input
      myName = name;
   }

   public String greet() {
      return "Hello "+myName;
   }
}
