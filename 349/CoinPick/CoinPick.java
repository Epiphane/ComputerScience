import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;

/**
 * CoinPick.java
 *
 * Problem: Given a set of coins, pick the coins that will give you
 * the highest value when you cannot pick 2 consecutive coins
 *
 * @author tsteinke
 *
 */

public class CoinPick {
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

    ArrayList<Integer> coins = new ArrayList<Integer>();
    System.out.print("Coins: ");
    while(input.hasNextInt()) {
      int newCoin = input.nextInt();
      System.out.print(newCoin + " ");
      coins.add(newCoin);
    }
    System.out.println();

    System.out.println("Picked indexes: " + pickCoins(coins));
  }

  public static ArrayList<Integer> pickCoins(ArrayList<Integer> coins) {
    if(coins.size() == 0)
      return null;

    int[] totalCount = new int[coins.size()];

    totalCount[0] = coins.get(0);

    int ndx;
    for(ndx = 1; ndx < coins.size(); ndx ++) {
      int coin = coins.get(ndx);
      if(ndx > 1 && coin + totalCount[ndx - 2] > totalCount[ndx - 1])
        totalCount[ndx] = coin + totalCount[ndx - 2];
      else
        totalCount[ndx] = totalCount[ndx - 1];
    }

    ArrayList<Integer> result = new ArrayList<Integer>();
    ndx --;
    while(ndx > 0) {
      if(totalCount[ndx - 1] == totalCount[ndx]) // Didn't pick me
        ndx --;
      else {
        // Picked me
        result.add(0, ndx);
        ndx -= 2;
      }
    }

    if(ndx == 0)
      result.add(0, ndx);

    return result;
  }
}

