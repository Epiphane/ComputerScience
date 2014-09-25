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
		
		System.out.println(firstDropHeight);
	}
}
