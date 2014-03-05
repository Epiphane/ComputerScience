import javax.swing.*;
import java.io.*;
import java.util.Scanner;

public class PrintPoem {
   public static void main(String[] args) {
      JFileChooser jfc = new JFileChooser();
      jfc.showOpenDialog(null);
      File f = jfc.getSelectedFile();
      String fileName = f.getName();
      
      try {
         PrintStream file = new PrintStream(f);
         System.out.println("Printing to file...");
         file.println("Mary had a little lamb");
         file.println("Its fleece was white as snow");
         file.println("And everywhere that Mary went");
         file.println("The lamb was sure to go!");
         file.close();
         System.out.println("Finished printing\n\nNow to read it:");
         
         Scanner readPoem = new Scanner(f);
         while(readPoem.hasNext()) {
            System.out.println(readPoem.nextLine());
         }
         readPoem.close();
      }
      catch(FileNotFoundException e) {
         System.out.println("ERROR: File disappeared");
         e.printStackTrace();
      }
   }
}