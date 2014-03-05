
public class TreeWork {
	public static <T extends Comparable<? super T>> boolean isHeap(T[] arr, int N) {
		// Cycle through loop for each node that necessarily has children.
		// You don't need to check N/2, rounded down
		for(int i = 0; i < N / 2; i ++) {
			T base = arr[i];
			T firstChild = arr[i*2 + 1];
			if(firstChild.compareTo(base) <= 0) return false;
			if(i*2 + 3 <= N) {
				T secondChild = arr[i*2 + 2];
				if(secondChild.compareTo(base) <= 0) return false;
			}
		}
		return true;
	}
	
	public static <T> void printTree(T[] arr, int N) {
		int nextTail = 0;
		for(int i = 0; i < N; i ++) {
			System.out.print(arr[i]+ " ");
			if(i == nextTail) {
				nextTail = nextTail * 2 + 2;
				System.out.println();
			}
		}
	System.out.println();
	}
}
