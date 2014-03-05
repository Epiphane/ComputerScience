import java.util.Scanner;

class DiGraphTest {
	public static void main(String[] args) {
		Scanner scanman = new Scanner(System.in);
		
		System.out.println("How large is the graph?");
		int N = scanman.nextInt();
		System.out.println(N);
		scanman.nextLine();
		
		DiGraphAM graph = new DiGraphAM(N);

		System.out.println("Choose one of the following operations:");
		System.out.println("- Add edge (enter the letter a)");
		System.out.println("- Delete edge (enter the letter d)");
		System.out.println("- Edge count (enter the letter e)");
		System.out.println("- Vertex count (enter the letter v)");
		System.out.println("- Print (enter the letter p)");
		System.out.println("- Topological sort (enter the letter t)");
		System.out.println("- Quit (enter the letter q)");
		
		String input;
		do {
			input = scanman.nextLine();
			char i = input.charAt(0);
			System.out.println(i);
			
			// Interpret the input
			switch(i) {
			case 'a':
				System.out.println("Enter the start and end vertices");
				if(scanman.hasNextInt()) {
					int from = scanman.nextInt();
					if(scanman.hasNextInt()) {
						int to = scanman.nextInt();
						System.out.println(from + " " + to);
						graph.addEdge(from, to);
						System.out.println("Edge between " + from + " and " + to + " added!");
					}
					else {
						System.out.println("That's no integer....");
					}
				}
				else {
					System.out.println("That's no integer....");
				}
				
				scanman.nextLine();
				break;
			case 'd':
				System.out.println("Enter the start and end vertices");
				if(scanman.hasNextInt()) {
					int from = scanman.nextInt();
					if(scanman.hasNextInt()) {
						int to = scanman.nextInt();
						System.out.println(from + " " + to);
						graph.deleteEdge(from, to);
						System.out.println("Edge between " + from + " and " + to + " deleted!");
					}
					else {
						System.out.println("That's no integer....");
					}
				}
				else {
					System.out.println("That's no integer....");
				}

				scanman.nextLine();
				break;
			case 'e':
				System.out.println("There are " + graph.edgeCount() + " edges!");
				break;
			case 'v':
				System.out.println("There are " + graph.vertexCount() + " vertices!");
				break;
			case 'p':
				System.out.println("The content of the adjacency graph is the following:");
				graph.print();
				break;
			case 't':
				int[] result;
				try {
					result = graph.topSort();
					System.out.print("Result of the topographical sort: ");
					for(int number : result) {
						System.out.print(number + " ");
					}
					System.out.println();
				} catch (Exception e) {
					System.out.println("It's a cyclical graph! I can't do that!");
				}
				break;
			case 'q':
				System.out.println("Quitting");
				break;
			default:
				System.out.println("Invalid character. ");
				break;
			}
		} while(!input.equals("q"));
	}
}