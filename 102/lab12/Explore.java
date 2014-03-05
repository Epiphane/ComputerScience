import java.util.*;

public class Explore {
   public static void main(String[] args) {
      LinkedList<String> stringList = new LinkedList<String>();
      LinkedList<Integer> intList = new LinkedList<Integer>();
      
      stringList.add("A");
      stringList.add("B");
      stringList.add("C");
      stringList.add("D");
      stringList.add("E");
      stringList.add("F");
      stringList.add("G");
      stringList.add("H");
      
      intList.add(1);
      intList.add(2);
      intList.add(3);
      intList.add(4);
      intList.add(5);
      intList.add(6);
      intList.add(7);
      intList.add(8);
      
      System.out.println("\nTesting Iterator...");
      Iterator<String> itString = stringList.iterator();
      while(itString.hasNext()) {
         System.out.println(itString.next());
      }
      
      System.out.println("\nTesting for(Integer n : intList)...");
      for(Integer n : intList) {
         System.out.println(n);
      }
      
      System.out.println("\nTesting Stack...");
      Stack<String> stack = new Stack<String>();
      stack.push("A");
      stack.push("B");
      stack.push("C");
      System.out.println(stack.pop());
      stack.push("D");
      System.out.println(stack.pop());
      System.out.println(stack.pop());
      System.out.println(stack.pop()); 
   }
}