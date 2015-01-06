import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;

/**
 * MergeAndCount.java
 *
 * Uses MergeSort to count the inversions in an array of integers
 *
 * @author tsteinke
 *
 */

public class MergeAndCount {
  public static void main(String[] args) {
    if(args.length == 0) {
      System.out.println("Provide the filename!");
      return;
    }

    Scanner scanman;
    try {
      scanman = new Scanner(new File(args[0]));
    }
    catch(Exception e) {
      e.printStackTrace();
      return;
    }
    int arrayLen = scanman.nextInt();

    // Put file input into array
    int[] array = new int[arrayLen];
    for(int i = 0; i < arrayLen; i ++) {
      array[i] = scanman.nextInt();
    }

    System.out.println("The number of inversions is " + countInversions(array, 0, arrayLen));
  }

  public static int countInversions(int[] array, int left, int right) {
    if(right - left == 1)
      return 0;

    int numInversions = 0;

    int median = (left + right) / 2;
    numInversions += countInversions(array, left, median);
    numInversions += countInversions(array, median, right);
    numInversions += mergeAndCountInversions(array, left, median, right);

    return numInversions;
  }

  public static int mergeAndCountInversions(int[] array, int left, int median, int right) {
    int numInversions = 0;

    int[] leftSide = new int[median - left];
    int[] rightSide = new int[right - median];

    // Copy arrays to temp other array
    for(int i = 0; i < leftSide.length; i ++)
      leftSide[i] = array[left + i];
    for(int i = 0; i < rightSide.length; i ++)
      rightSide[i] = array[median + i];

    // Begin the merge!
    int l = 0, r = 0;
    while(l < leftSide.length && r < rightSide.length) {
      // No new inversions here
      if(rightSide[r] < leftSide[l]) {
        array[left + r + l] = rightSide[r];
        r ++;
      }
      // Adding inversions!!
      else {
        array[left + r + l] = leftSide[l];
        l ++;
        numInversions += r;
      }
    }

    while(l < leftSide.length) { // r == rightSide.length
      array[left + l + r] = leftSide[l];
      l ++;
      numInversions += r;
    }

    while(r < rightSide.length) {
      array[left + l + r] = rightSide[r];
      r ++;
    }

    return numInversions;
  }
}
