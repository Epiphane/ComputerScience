import java.lang.Math;

public class You {
	private int age, height;
	private int str, smarts;
	private int health;
	
	private String name;
	private String species;
	private String status;
	
	public You(String name) {
		age = (int) (Math.random() * 30);
		height = ((int) (Math.random() * 36))+48;
	
		this.name = name;
		status = "Mediocre";
		species = "Human";
		
		str = 100;
		smarts = 100;

		if(name.equals("Elliot")) {
			smarts = 150;
			str = 50;
		}
	}
	
	public String getName() {
		return name;
	}
	
	public void printBio() {
		System.out.println("Your name is "+name+". You are "+age+" years old.");
		System.out.println("You are a "+status+" "+species);
		System.out.println("Height: "+height/12+" feet, "+height%12+" inches");
		
		System.out.print("Strength: "+str+". ");
		if(str < 70)
			System.out.println("You're ridiculously pathetic...");
		else if(str < 100)
			System.out.println("You're totally mediocre.");
		else if(str < 120)
			System.out.println("Do you even lift?");
		else if(str < 150)
			System.out.println("You're pretty damn strong!");
		else
			System.out.println("You're a fucking god among men.");
			
		System.out.print("Smarts: "+smarts+" IQ. ");
		if(smarts < 70)
			System.out.println("Can you even read this? Seriously, you're more retarded than retarded");
		else if(smarts < 100)
			System.out.println("You're a complete idiot");
		else if(smarts < 120)
			System.out.println("You're average.");
		else if(smarts < 150)
			System.out.println("You're pretty damn smart.");
		else
			System.out.println("You're a fucking god among men.");
		
		System.out.println();
	}
}
