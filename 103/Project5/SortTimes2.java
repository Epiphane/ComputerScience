/****
 * Class SortTimes2 runs a test of the 5 different Sorts
 * methods, using a list of unsorted integers to test
 * the running times of the sorts. Tests each
 * N from 5000 to 80000 (doubling each time).
 * 
 * @author Thomas Steinke, Elliot Fiske
 */

public class SortTimes2 {
	public static void main(String[] args) {
		// Set up array for testing
		Integer[] listA = new Integer[800000];
		Integer[] listB = new Integer[800000];
		Integer[] listC = new Integer[800000];
		Integer[] listD = new Integer[800000];
		Integer[] listE = new Integer[800000];
		
		// Time results
		long[] times = new long[5];
		
		System.out.println("\nTEST2: reverse-sorted list");
		for(int N = 5000; N <= 80000; N *= 2) {
			// Fill array
			for(int i = 0; i < N; i ++) {
				listA[i] = N - i;
			}
			
			// Copy array
			for(int i = 0; i < N; i ++) {
				listB[i] = listA[i];
				listC[i] = listA[i];
				listD[i] = listA[i];
				listE[i] = listA[i];
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
		
		System.out.println("END TEST2");
	}
}
