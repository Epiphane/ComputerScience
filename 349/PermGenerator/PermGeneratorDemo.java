import java.util.Scanner;
import java.util.ArrayList;

/**
 * PermGeneratorDemo.java
 *
 * Problem: Given a string, construct all permutations
 * of it in both lexigraphic and minimum-change order.
 * Algorithms are in LexPermGenerator and MinChangePermGenerator classes
 *
 * @author tsteinke
 *
 */

public class PermGeneratorDemo {
	public static void main(String[] args) {
    String inputString;

		Scanner scanman = new Scanner(System.in);

		System.out.println("Enter string to permutate: ");
		inputString = scanman.next();
    System.out.println();

		System.out.println("The permutations of the letters in " + inputString + " in lexigraphic order are: ");
    ArrayList<String> permutations = LexPermGenerator.getPermutations(inputString);
    printPermutations(permutations);

		System.out.println("The permutations of the letters in " + inputString + " with \"minimum-change\" are: ");
    permutations = MinChangePermGenerator.getPermutations(inputString);
    printPermutations(permutations);
	}

  public static void printPermutations(ArrayList<String> permutations) {
    for(String perm : permutations) {
      System.out.println(perm);
    }
    System.out.println();
  }
}
