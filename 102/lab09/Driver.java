import java.util.Scanner;

public class Driver {
   public static void main(String[] args) {
      ThreeBadFunctions tbf = new ThreeBadFunctions();
      try {
         Scanner in = new Scanner(System.in);
         
         System.out.println("What function should be run? (1,2,3,4,5)");
         tbf.doStuff(in.nextInt());
      }
      catch(NullPointerException e) {
         System.out.println("Why did you pick 1? You knew it would cause a NullPointerException");
         e.printStackTrace();
      }
      catch(ClassCastException e) {
         System.out.println("You picked 2? You idiot...it causes ClassCastException!");
         e.printStackTrace();
      }
      catch(ArrayIndexOutOfBoundsException e) {
         System.out.println("Why in God's name would you ever pick 3..."+
                            "it causes ArrayIndexOutOfBoundsException");
         e.printStackTrace();
      }
      catch(DumbCallerException e) {
         System.out.println("YOU DUMMY! This function just throws an exception,"+
                            "What did you expect?");
         e.printStackTrace();
      }
      catch(RandomRuntimeException e) {
         System.out.println("...Well You threw RandomRuntimeException. Congrats");
         e.printStackTrace();
      }
      finally {
         System.out.println("I know you messed up, but it's ok. I forgive you");
      }
   }
}