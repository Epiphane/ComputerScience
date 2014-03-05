/**
 * DO NOT MODIFY THIS FILE!
 *
 * @author Kurt Mammen
 * @version Soda Machine Project
 * @version CPE 102
 */
import javax.swing.JFrame;

public class SodaMachineDriver
{
   /**
    * The entry point for Soda Machine Program
    * @param args The array of command line arguments, if any.
    */
   public static void main(String[] args)
   {
      SodaMachine machine = new SodaMachine();

      JFrame appFrame = new SodaMachineFrame(machine);
      appFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      appFrame.setVisible(true);
   }
}
