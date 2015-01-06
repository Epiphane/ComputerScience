import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * FraudDetector.java
 *
 * Problem: Some counterfeit signature may make up more than n/2
 * transactions. Using Divide and conquer, find the signature
 *
 * @author tsteinke
 *
 */

public class FraudDetector {
  private static class Counterfeit {
    public int ndx;
    public int count;

    public Counterfeit(int _ndx, int _count) {
      ndx = _ndx;
      count = _count;
    }
  }

	public static void main(String[] args) {
    if(args.length == 0) {
      System.out.println("Provide the filename!");
      return;
    }

    // Input the transactions
    Scanner input;
    try {
      input = new Scanner(new File(args[0]));
    }
    catch(FileNotFoundException e) {
      e.printStackTrace();
      return;
    }
    int numTransactions = input.nextInt();
    int transactions[] = new int[numTransactions];
    for(int i = 0; i < numTransactions; i ++)
      transactions[i] = input.nextInt();

    // Output time
    System.out.println("Threshold to determine fraudulent transactions is >= " + (numTransactions / 2 + 1));

    Counterfeit counterfeit = findCounterfeit(transactions, 0, numTransactions);
    if(counterfeit == null)
      System.out.println("The number of identical transaction credentials was insufficient");
    else {
      System.out.println("The number of identical transaction credentials was " + counterfeit.count);
      System.out.println("The index of one of the identical transaction credentials is: " + counterfeit.ndx);
    }
  }

  private static Counterfeit findCounterfeit(int[] transactions, int start, int end) {
    // Base case: 1 element
    if(end - start <= 1) {
      return new Counterfeit(start, 1);
    }

    // More than 1 element: divide and conquer
    int middle = (start + end) / 2;
    Counterfeit leftHalf = findCounterfeit(transactions, start, middle);
    Counterfeit rightHalf = findCounterfeit(transactions, middle, end);

    Counterfeit result = null;
    if(leftHalf != null) {
      int fake = transactions[leftHalf.ndx];
      for(int i = middle; i < end; i ++)
        if(fake == transactions[i])
          leftHalf.count ++;

      result = leftHalf;
    }
    if(rightHalf != null) {
      int fake = transactions[rightHalf.ndx];
      for(int i = start; i < middle; i ++)
        if(fake == transactions[i])
          rightHalf.count ++;

      if(leftHalf == null || rightHalf.count > leftHalf.count)
        result = rightHalf;
    }

    if(result != null && result.count > middle - start)
      return result;
    else return null;
  }
}
