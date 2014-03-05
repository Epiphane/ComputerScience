import java.util.Scanner;

public class Manifestation {
   public static void main(String[] args) {
	   Scanner scanner = new Scanner(System.in);
		
		System.out.println("AHAHAHAHAHAHA YOU ARE TRAPPED IN AN ENDLESS BATTLE OF BOREDOM.");
		while(true) {
			System.out.print("IS YOUR ANUS PREPARED (Y/N)?");
			
			String answer = scanner.next();
			
			if(answer.equals("Y")) {
			   System.out.println("GOOD.");
				break;
			}
			else {
			   System.out.println("THEN PREPARE IT.");
			}
		}
		
		sectionBreak();
		
		System.out.println("What is your name, scrub?");
		String name;

		while(true) {		
			name = scanner.next();	
			if(name.length() > 16) {
				System.out.println("Your name is too long boy. Fix it.");
			}
			else if(name.length() < 2) {
				System.out.println("Your name is too short son. Fix it, mister.");
			}
			else {
				break;
			}
				
			System.out.println("What is your name, scrub?");
		}
		
		You you = new You(name);
		Guide guide = new Guide();
	
		sectionBreak();

		World world = new AimlessWorld(10);
		
		world.runWorld(you,guide,scanner);
	}
	
	public static void sectionBreak() {
	
	   System.out.println("\n___________________________________________________");
	   System.out.println("---------------------------------------------------");
	}
}
