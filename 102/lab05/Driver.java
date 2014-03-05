import java.util.Scanner;

public class Driver {
   public static void main(String[] args) {
      Scanner in = new Scanner(System.in);
      
      ArrayListFun arrayListFun = new ArrayListFun();
      
      while(true) {
         System.out.print("Next value: ");
         if(in.hasNextInt()) {
            arrayListFun.add(in.nextInt());
            in.nextLine();
         }
         else if(in.hasNextDouble()) {
            arrayListFun.add(in.nextDouble());
            in.nextLine();
         }
         else if(in.hasNextBoolean()) {
            arrayListFun.add(in.nextBoolean());
            in.nextLine();
         }
         else if(in.hasNext()) {
            String input = in.nextLine();
            if(input.equals("quit")) {
               break;
            }
            
            arrayListFun.add(input);
         }
      }
      
      System.out.println("Minimum Int: "+arrayListFun.minimumInt());
      System.out.println("Average Double: "+arrayListFun.averageDouble());
      System.out.println("Number of Trues: "+arrayListFun.numberOfTrues());
      System.out.println("Number of Strings: "+arrayListFun.numberOfStrings());
   }
}