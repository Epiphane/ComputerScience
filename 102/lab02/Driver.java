// This import statement must appear outside and before the class.
import java.util.Scanner;

public class Driver {
   // This is the starting point of the program.
   public static void main(String[] args) {
      // Declare a Scanner reference and initialize it by constructing
      // a Scanner object that will read from the command-line.
      // Note: This is an example of a constructor call - very common in
      //       Object Oriented languages like Java.
      Scanner scanner = new Scanner(System.in);

      // Prompt the user for a name.
      // Note: This is an example of how you print to the command line in Java.
      System.out.print("What is your name? ");

      // Declare a String reference and initialize it with the name
      // that the user typed in response to the prompt.
      // Note: This is an example of how you call methods in Java.
      String name = scanner.nextLine();

      // Construct a Greeter object - this is a class you will write!
      Greeter greeter = new Greeter(name);

      // Declare a String reference and initialize it with the greeting
      // returned by a call to the Greeter class's greet method.
      String greeting = greeter.greet();

      // Display the greeting to the command-line
      System.out.println(greeting);
   }
}
