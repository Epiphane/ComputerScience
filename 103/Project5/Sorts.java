/****
 * Class Sorts provides utilities to sort lists using Selection, Bubble,
 * Insertion, Merge, and Quick Sorts
 * 
 * @author Thomas Steinke, Elliot Fiske
 */

public class Sorts {
	/**
	 * Sorts the list of size elements contained in arr using the selection sort algorithm
	 * O(N^2)
	 */
	public static <Type extends Comparable<? super Type>> void selectionSort(Type[] arr, int size) {
		// Start at 0 and go through all the new positions for items
		for (int i = 0; i < size; i++) {
			int minIndex = i;
			// Look for something lower than the current value
			for (int j = i+1; j < size; j++) {
				if (arr[j].compareTo(arr[minIndex]) < 0) {
					minIndex = j;
				}
			}
			
			// And swap it
			swap(arr, i, minIndex);
		}
	}

	/**
	 * Sorts the list of size elements contained in arr using the bubble sort algorithm
	 * O(N^2), best case O(N)
	 */
	public static <Type extends Comparable<? super Type>> void bubbleSort(Type[] arr, int size) {
		boolean done = false;
		while(!done) {
			done = true;
			for (int i = 0; i < size-1; i++) {
				if(arr[i].compareTo(arr[i+1]) > 0) {
					swap(arr, i, i+1);
					done = false;
				}
			}
		}
	}
	
	/**
	 * Sorts the list of size elements contained in arr using the insertion sort algorithm
	 * O(N^2), best case O(N)
	 */
	public static <Type extends Comparable<? super Type>> void insertionSort(Type[] arr, int size) {
		for (int i = 1; i < size; i++) {
			Type temp = arr[i];
			int j = i;
			// Move back to the beginning, swapping it with everything greater than it
			while(j > 0 && arr[j-1].compareTo(temp) > 0) {
				swap(arr, j, --j);
			}
			
			arr[j] = temp;
		}
	}

	/**
	 * Sorts the list of size elements contained in arr using the merge sort algorithm
	 * O(NlogN)
	 */
	public static <Type extends Comparable<? super Type>> void mergeSort(Type[] arr, int size) {
		mergeSort(arr, 0, size-1);
	}

	private static <Type extends Comparable<? super Type>> void mergeSort(Type[] arr, int first, int last) {
		// Only one element
		// ? Return
		if(last == first) return;
		
		// Split the array and merge sort again
		int middle = (first+last) / 2;
		mergeSort(arr, first, middle);
		mergeSort(arr, middle+1, last);
		
		mergeSortedHalves(arr, first, middle, last);
	}

	private static <Type extends Comparable<? super Type>> void mergeSortedHalves(Type[] arr, int first, int middle, int last) {
		Type[] temp = (Type[]) new Comparable[last - first + 1];
		int index1 = first;
		int index2 = middle + 1;
		int index = 0;
		
		while(index1 < middle + 1 && index2 < last + 1) {
			if(arr[index1].compareTo(arr[index2]) < 0) {
				temp[index++] = arr[index1++];
			}
			else {
				temp[index++] = arr[index2++];
			}
		}
		
		// Extra at the end of either half?
		for(;index1 < middle+1; index1++) {
			temp[index++] = arr[index1];
		}
		for(;index2 < last+1; index2++) {
			temp[index++] = arr[index2];
		}
		
		for(int i = 0; i < temp.length; i++) {
			arr[i+first] = temp[i];
		}
	}

	/**
	 * Sorts the list of size elements contained in arr using the quick sort algorithm
	 * with median-of-three pivot and rearrangement of the three elements
	 * O(NlogN), worst case O(N^2)
	 */
	public static <Type extends Comparable<? super Type>> void quickSort(Type[] arr, int size) {
		quickSort(arr, 0, size-1);
	}
	

	private static <Type extends Comparable<? super Type>> void quickSort(Type[] arr, int first, int last) {
		if(first < last) {
			setPivotToEnd(arr, first, last);
			
			// Quicksort both parts of the array
			int pivotIndex = splitList(arr, first, last);
			quickSort(arr, first, pivotIndex - 1);
			quickSort(arr, pivotIndex + 1, last);
		}
	}

	private static <Type extends Comparable<? super Type>> void setPivotToEnd(Type[] arr, int first, int last) {
		int middle = (first+last) / 2;
		// Middle < First
		if(arr[middle].compareTo(arr[first]) < 0) {
			swap(arr, middle, first);
		}
		// Last < First
		if(arr[last].compareTo(arr[first]) < 0) {
			swap(arr, last, first);
		}
		// Middle < Last
		if(arr[middle].compareTo(arr[last]) < 0) {
			swap(arr, middle, last);
		}
	}

	private static <Type extends Comparable<? super Type>> int splitList(Type[] arr, int first, int last) {
		int indexL = first;
		int indexR = last-1;
		
		Type pivot = arr[last];
		while(indexR > indexL) {
			while(arr[indexL].compareTo(pivot) < 0) {
				indexL ++;
			}
			while(indexR > indexL && arr[indexR].compareTo(pivot) > 0) {
				indexR --;
			}

			if(indexR > indexL) {
				swap(arr, indexL++, indexR--);
			}
		}
		
		swap(arr, indexL, last);
		return indexL;
	}
	
	/**
	 * Swaps any 2 given indexes in an array
	 */
	private static <Type extends Comparable<? super Type>> void swap(Type[] arr, int i, int j) {
		Type t = arr[i];
		arr[i] = arr[j];
		arr[j] = t;
	}
}
