import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;


public class TreeTest {
	public static void main(String[] args) {
		Integer[] tree = new Integer[20];
		int size = 0;
		
		Scanner in;
		if(args.length > 0) {
			try {
				//System.out.println(args[0]);
				in = new Scanner(new File(args[0]));
			} catch (FileNotFoundException e) {
				System.out.println("File not found.");
				return;
			}
		}
		else in = new Scanner(System.in);

		System.out.println("Enter characters for the heap: ");
		while(size < 20 && in.hasNext()) {
			tree[size] = in.nextInt();
			if(args.length > 0) {
				System.out.print(tree[size] + " ");
			}
			size ++;
		}
		if(args.length > 0) System.out.println();
		
		in.close();
		if(size == 0) {
			System.out.println("There isn't anything there!!");
			return;
		}
		
		if(TreeWork.isHeap(tree, size)) {
			System.out.println("That is most DEFINITELY a heap!");
		}
		else {
			System.out.println("That's no heap...");
		}
		
		TreeWork.printTree(tree, size);
	}
}
