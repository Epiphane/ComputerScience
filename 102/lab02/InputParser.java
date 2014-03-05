import java.util.Scanner;

public class InputParser {
   public static void main(String[] args) {
      Scanner scanner = new Scanner(System.in);
      
      System.out.print("How many values would you like to parse? ");
      //Receive amount of values to parse
      int amountOfValues = scanner.nextInt();

      int integerSum = 0;      // Holds the sum of the integers,
      double doubleSum = 0;    // Doubles,
      String stringSum = "";   // And Strings

      int[] indices = new int[] {0,0,0};   // Keeps track of how many integers (indices[0]),
                                           // doubles (indices[1]), and strings (indices[2])
                                           // there are
      
      // First we get the integers, strings, and doubles
      System.out.println("\n---------------------------------------------------------------");
      while(amountOfValues-- > 0) {
         System.out.print("Next value: ");
         // Determine whether input is an integer, double, or string
         // And add it to that respective array
         if(scanner.hasNextInt()) {
            integerSum += scanner.nextInt();
            indices[0]++;
         }
         else if(scanner.hasNextDouble()) {
            doubleSum += scanner.nextDouble();
            indices[1]++;
         }
         else {
            stringSum += scanner.next();
            indices[2]++;
         }
      }

      // And now we display the integers, strings, and doubles...
      System.out.println("\n---------------------------------------------------------------");
      if(indices[0] > 0) {
         System.out.print("You entered "+(indices[0])+" integers. ");
         System.out.println("Their sum is:           "+integerSum);
      }
      if(indices[1] > 0) {
         System.out.print("You entered "+(indices[1])+" doubles. ");
         System.out.println("Their sum is:            "+doubleSum);
      }
      if(indices[2] > 0) {
         System.out.print("You entered "+(indices[2])+" strings. ");
         System.out.println("Concantenated, they say: "+stringSum);
      }
   }
}
