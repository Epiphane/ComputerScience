import java.util.Scanner;

public class AimlessWorld extends World {
	private Pidgeon[] pidgeons;

	public AimlessWorld(int numberOfPidgeons) {
	   pidgeons = new Pidgeon[numberOfPidgeons];
		
		for(int i = 0; i < pidgeons.length; i ++) {
			pidgeons[i] = new Pidgeon();
		}
	}
	
	public void runWorld(You you, Guide guide, Scanner scanner) {
		System.out.println("You find yourself in the large and diverse world of Antiscopia.");
		System.out.println("\nThis is you:");
		you.printBio();

		guide.prompt();

		guide.greet(you.getName());

		guide.prompt();

		System.out.println("\nThere are a few Pidgeons around, lazily milling about and talking about the most random of topics. One walks up to you.");
		
		guide.prompt();

		Pidgeon greeterPidgeon = pidgeons[0];

		greeterPidgeon.greet();
	}
}
