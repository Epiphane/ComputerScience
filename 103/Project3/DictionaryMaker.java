import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.Iterator;
import java.util.Scanner;

/****
 * 
 * Class DictionaryMaker reads a text file and generates a lexicographically
 * sorted listing (increasing order) of all distinct "words" contained in that
 * file (duplicates ignored). Output is to go to a text file indicated by user.
 * 
 * @author Thomas Steinke, Elliot Fiske
 */
public class DictionaryMaker {
	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {
		/** This scanner reads the file that the student data is stored in. */
		Scanner scanin = new Scanner(System.in);

		//Prompt the user
		System.out.print("WHERE ARE THE FILES STORED? (type your answer) ");

		/** This scanner takes in the file input to be processed. */
		Scanner fileScanner;
		while(true) {
			try {
				fileScanner = new Scanner(new File(scanin.nextLine()), "UTF-8");
				break;
			} catch (FileNotFoundException e) {
				System.out.println("The file was not there! Try again!");
				System.out.print("WHERE ARE THE FILES STORED? (type your answer) ");
			}
		}


		// Initialize file writer
		System.out.print("Enter filename to output to: ");

		PrintWriter putterman = null;
		try {
			String fileName = scanin.nextLine();
			System.out.println(fileName);
			putterman = new PrintWriter(fileName, "UTF-8");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		} 

		// Create empty tree
		BST<String> theTree = new BST<String>();

		// Insert everything from scanman into the tree
		while(fileScanner.hasNext()) {
			String searchForMe = fileScanner.next();
			if(!theTree.find(searchForMe))
				theTree.insert(searchForMe);
		}

		// Print tree to output file
		Iterator<String> iter = theTree.iteratorIn();
		while(iter.hasNext()) {
			String next = iter.next();
			putterman.println(next);
		}

		putterman.close();
	}
}
