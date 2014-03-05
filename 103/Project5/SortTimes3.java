/****
 * Class SortTimes3 runs a test of the 5 different Sorts
 * methods, using a list of unsorted integers to test
 * the running times of the sorts. Tests each
 * N from 5000 to 80000 (doubling each time) three times.
 * 
 * @author Thomas Steinke, Elliot Fiske
 */

import java.util.Random;

public class SortTimes3 {
	public static void main(String[] args) {
		Random randomater = new Random();
		
		// Set up array for testing
		Integer[] listA = new Integer[800000];
		Integer[] listB = new Integer[800000];
		Integer[] listC = new Integer[800000];
		Integer[] listD = new Integer[800000];
		Integer[] listE = new Integer[800000];	
	
		// Time results
		long[] times = new long[5];
		
		System.out.println("\nTEST3: unsorted list");
		for(int N = 5000; N <= 80000; N *= 2) {
			for(int testNum = 0; testNum < 3; testNum ++) {
				// Fill array
				for(int i = 0; i < N; i ++) {
					listA[i] = randomater.nextInt(N-1);
					listB[i] = randomater.nextInt(N-1);
					listC[i] = randomater.nextInt(N-1);
					listD[i] = randomater.nextInt(N-1);
					listE[i] = randomater.nextInt(N-1);
				}
				
				// Copy array
				for(int i = 0; i < N; i ++) {
						listA[i] = listA[i];
						listB[i] = listB[i];
						listC[i] = listC[i];
						listD[i] = listD[i];
						listE[i] = listE[i];
				}

				long currentTime = System.currentTimeMillis();
	
				// First Sort: Selection
				Sorts.selectionSort(listA, N);
				times[0] = System.currentTimeMillis() - currentTime;
				
				// Second Sort: Bubble
				currentTime = System.currentTimeMillis();
				Sorts.bubbleSort(listB, N);
				times[1] = System.currentTimeMillis() - currentTime;
	
				// Third Sort: Insertion
				currentTime = System.currentTimeMillis();
				Sorts.insertionSort(listC, N);
				times[2] = System.currentTimeMillis() - currentTime;
	
				// Fourth Sort: Merge
				currentTime = System.currentTimeMillis();
				Sorts.mergeSort(listD, N);
				times[3] = System.currentTimeMillis() - currentTime;
	
				// Fifth Sort: Quick
				currentTime = System.currentTimeMillis();
				Sorts.quickSort(listE, N);
				times[4] = System.currentTimeMillis() - currentTime;
				
				System.out.println("N="+N+": T_ss="+times[0]+", T_bs="+times[1]+", T_is="+times[2]+", T_ms="+times[3]+", T_qs="+times[4]);
			}
			System.out.println();
		}
		
		System.out.println("END TEST3");
	}
}
