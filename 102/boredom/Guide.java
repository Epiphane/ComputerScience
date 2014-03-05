import java.util.Scanner;

public class Guide {
	String guideName;
	Scanner in;

	Guide() {
		in = new Scanner(System.in);
		guideName = "Bubba";
	}
	
	public void greet(String name) {
		System.out.println("Hello "+name+"! I'm "+guideName+", and I will be guiding you");
		System.out.println("There's plenty of stuff you can do, but I'm not going to tell you anything");
		System.out.println("...oh fine. Say 'help' if you need, well, help. I won't tell you anything you don't already know though!");
	}

	public void prompt() {
		in.nextLine();
	}
}
