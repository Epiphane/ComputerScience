import java.util.Scanner;

/**
 * DestTestClass.java
 * 
 * Problem: Given a ladder height, determine the highest rung
 * on the ladder from which you can drop some equipment without
 * breaking it. You only have two pieces of equipment.
 * 
 * @author tsteinke
 *
 */

public class DestTestClass {
	public static void main(String[] args) {
		int ladderHeight, highestSafeRung;
		int totalDrops = 0;
		
		Scanner scanman = new Scanner(System.in);
		
		System.out.println("Enter height of the ladder: ");
		ladderHeight = scanman.nextInt();

		System.out.println("Enter highest safe rung on the ladder: ");
		highestSafeRung = scanman.nextInt();
		
		System.out.println("The height of the ladder is " + ladderHeight + ", the highest safe rung is " + highestSafeRung);
		
		// Begin algorithm. Find the drop height we want to start from
		int firstDropHeight = 0;
		int totalDropHeights = 0;
		while(totalDropHeights < ladderHeight) {
			firstDropHeight ++;
			totalDropHeights += firstDropHeight;
		}
		
		// Actual algorithm: Move up firstDropHeight, then firstDropHeight - 1, etc...
		totalDropHeights = 0; // What rung we're standing on
		while(totalDrops ++ && totalDropHeights <= highestSafeRung) {
			// Move up and drop again...
			totalDropHeights += firstDropHeight --;
		}
		
		// First break! Go back down almost to the last rung we tested on.
		int firstBreak = totalDropHeights;
		totalDropHeights -= firstDropHeight;
		
		// Drop it one rung at a time
		while(totalDrops ++ && totalDropHeights <= highestSafeRung) {
			// move up
			totalDropHeights ++;
		}
		
		// Second break is one rung below us!
		System.out.println("Highest safe rung determined by this experiment: " + totalDropHeights - 1);
		System.out.println("Rung where the first test device broke: " + firstBreak);
		System.out.println("Rung where the second test device broke: " + totalDropHeights);
		System.out.println("Total number of drops: " + totalDrops);
	}
}
