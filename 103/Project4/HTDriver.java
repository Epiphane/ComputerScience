/****
 * @author efiske, tsteinke, Elliot Fiske, Thomas Steinke
 * 
 * Class HTDriver will read in a file of student IDs and names and allow you to modify the hash table afterwards
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class HTDriver {
	public static void main(String[] args) {
		Scanner consoleman = new Scanner(System.in);
		//Prompt the user
		System.out.print("WHERE ARE THE FILES STORED? (type your answer) ");

		/** This scanner takes in the file input to be processed. */
		Scanner fileScanner;
		while(true) {
			try {
				fileScanner = new Scanner(new File(consoleman.nextLine()));
				break;
			} catch (FileNotFoundException e) {
				System.out.println("The file was not there! Try again!");
				System.out.print("WHERE ARE THE FILES STORED? (type your answer) ");
			}
		}

		int N = fileScanner.nextInt();
		HashTable hashTable = new HashTable(N);

		//This regex pattern recognizes a student ID number followed by a name.
		Pattern recordPattern = Pattern.compile("([1-9][0-9]*) +([A-z]+)");
		Matcher recordMatcher = null;

		while(fileScanner.hasNext()) {
			//Examine each line in the file using our regular expression
			recordMatcher = recordPattern.matcher(fileScanner.nextLine());
			
			if(recordMatcher.matches()) {
				//There are two backreferences from the regular expression;
				//The first is the student ID
				long id = Long.parseLong(recordMatcher.group(1));
				//And the second is the student's last name.
				String name = recordMatcher.group(2);
				hashTable.insert(new Student(id, name));
			}
		}

		System.out.println("Choose one of the following operations by entering the provided letter:");
		System.out.println("\ta - Add the element");
		System.out.println("\td - Delete the element");
		System.out.println("\tf - Find and retrieve the element");
		System.out.println("\tn - Get the size of the collection");
		System.out.println("\te - Check if the collection is empty");
		System.out.println("\tk - Make the hash table empty");
		System.out.println("\tp - Print the contents of the hash table");
		System.out.println("\to - Output the elements of the collection");

		System.out.println("\tq - Quit");

		String input;
		do {
			System.out.print("\nPlease enter a command: ");
			input = consoleman.nextLine();
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
				System.out.print("Enter Student in the following format [ ID Name ] ");
				//Examine each line in the file using our regular expression
				recordMatcher = recordPattern.matcher(consoleman.nextLine());
				
				long id;
				if(recordMatcher.matches()) {
					//There are two backreferences from the regular expression;
					//The first is the student ID
					id = Long.parseLong(recordMatcher.group(1));
					//And the second is the student's last name.
					String name = recordMatcher.group(2);
					hashTable.insert(new Student(id, name));
					System.out.println(name + " was added to the table");
				}
				else {
					System.out.println("Incorrect format.");
				}
				break;
			case 'd':
				System.out.print("What ID should I delete? ");
				if(consoleman.hasNextLong()) {
					Integer myInt = consoleman.nextInt();
					hashTable.delete(new Student(myInt,""));
					System.out.println(myInt + " was deleted.");
				} else {
					System.out.println("That's no long....");
				}
				consoleman.nextLine();
				break;
			case 'f':
				System.out.print("What ID should I find? ");
				if(consoleman.hasNextLong()) {
					Integer myInt = consoleman.nextInt();
					Object student = hashTable.find(new Student(myInt,""));
					if(student == null) {
						System.out.println("Student not found!");
					}
					else {
						System.out.println("We have located your student: "+student);
					}
				} else {
					System.out.println("That's no long....");
				}
				consoleman.nextLine();
				break;
			case 'n':
				System.out.println("The hash table has "+hashTable.elementCount()+" nodes");
				break;
			case 'e':
				System.out.println("The hash table is "+ (hashTable.isEmpty() ? "" : "NOT " ) + "empty!");
				break;
			case 'k':
				hashTable.makeEmpty();
				System.out.println("Hash Table is now empty");
				break;
			case 'p':
				hashTable.printTable();
				break;
			case 'o':
				Iterator<Object> it = hashTable.iterator();
				while(it.hasNext()) {
					System.out.println(it.next());
				}
				break;
			case 'q':
				break;
			case ';':
				break;
			default:
				System.out.println("Invalid character. ");
				break;
			}
		} while(!input.equals("q"));
		System.out.println("Farewell!");

		consoleman.close();
	}
}
