/****
 * Class BSTDriver actually drives the program. It uses BST, which is where the REAL work is.
 * 
 * @author Thomas Steinke, Elliot Fiske
 */
import java.util.Iterator;
import java.util.Scanner;

public class BSTDriver {
	public static void main(String[] args) {
		BST<Integer> freeTree = new BST<Integer>();

		Scanner scanman = new Scanner(System.in);

		System.out.println("Choose one of the following operations by entering the provided letter:");
		System.out.println("\ta - Add the element");
		System.out.println("\td - Delete the element");
		System.out.println("\tf - Find the element");
		System.out.println("\te - Check if the tree is empty");
		System.out.println("\tk - Make the tree empty");
		System.out.println("\tn - Get the number of nodes (the size) of the tree");
		System.out.println("\tm - Find the minimum element");
		System.out.println("\tx - Find the maximum element");
		System.out.println("\tp - Print the tree in preorder using iterator");
		System.out.println("\ti - Print the tree in inorder using iterator");
		System.out.println("\tl - Print the tree in levelorder using iterator");
		System.out.println("\tt - Print the tree using printTree");
		System.out.println("\to - Output the tree using toString");

		System.out.println("\tq - Quit");

		String input;
		do {
			System.out.print("\nPlease enter a command: ");
			input = scanman.nextLine();
			System.out.println(input); // TODO: DELETE
			char i = input.charAt(0);

			//if(input.length() != 1) { 
			//	System.out.println("Invalid character selection");
			//	continue;
			//}
			
			Iterator<Integer> iter;

			// Interpret the input
			switch(i) {
			case 'a':
				System.out.print("What Integer should I add? ");
				if(scanman.hasNextInt()) {
					Integer myInt = scanman.nextInt();
					freeTree.insert(myInt);
					System.out.println(myInt + " was added.");
				} else {
					System.out.println("That's no integer....");
				}
				scanman.nextLine();
				break;
			case 'd':
				System.out.print("What Integer should I delete? ");
				if(scanman.hasNextInt()) {
					Integer myInt = scanman.nextInt();
					freeTree.delete(myInt);
					System.out.println(myInt + " was deleted.");
				} else {
					System.out.println("That's no integer....");
				}
				scanman.nextLine();
				break;
			case 'f':
				System.out.print("What Integer should I find? ");
				if(scanman.hasNextInt()) {
					Integer myInt = scanman.nextInt();
					System.out.println(myInt+" was " + (freeTree.find(myInt) ? "" : "NOT ") + "found!");
				} else {
					System.out.println("That's no integer....");
				}
				scanman.nextLine();
				break;
			case 'e':
				System.out.println("The list is " + (freeTree.isEmpty() ? "" : "NOT ") + "empty.");
				break;
			case 'k':
				freeTree.makeEmpty();
				System.out.println("Tree now empty");
				break;
			case 'n':
				System.out.println("The tree has "+freeTree.size()+" nodes");
				break;
			case 'x':
				if(freeTree.isEmpty()) {
					System.out.println("Error: Tree is empty!");
				} else {
					System.out.println("List Maximum: " + freeTree.findMaximum());
				}
				break;
			case 'm':
				if(freeTree.isEmpty()) {
					System.out.println("Error: Tree is empty!");
				} else {
					System.out.println("List Minimum: " + freeTree.findMinimum());
				}
				break;
			case 'p':
				iter = freeTree.iteratorPre();
				while(iter.hasNext()) System.out.print(iter.next()+" ");
				System.out.println();
				break;
			case 'i':
				iter = freeTree.iteratorIn();
				while(iter.hasNext()) System.out.print(iter.next()+" ");
				System.out.println();
				break;
			case 'l':
				iter = freeTree.iteratorLevel();
				while(iter.hasNext()) System.out.print(iter.next()+" ");
				System.out.println();
				break;
			case 't':
				freeTree.printTree();
				break;
			case 'o':
				System.out.println(freeTree);
				break;
			case 'q':
				System.out.println("Quitting");
				break;
				// TODO: REMOVE THE SEMICOLON SHIT FUCK NONSENSEFUCKER
				// OR YOUR ASS SHALL BE SHAT UPON BY THE ALMIGHTY
			case ';':
				break;
			default:
				System.out.println("Invalid character. ");
				break;
			}
		} while(!input.equals("q"));
		System.out.println("Done!");

		scanman.close();
	}
}
