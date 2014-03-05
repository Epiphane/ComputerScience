/**
 * Provided basic compile test for Box quiz. 
 *
 * Verifies that all expected methods are present and that they potentially
 * throw the expected exceptions.
 *
 * ************************************************************************
 * *** NOTE THAT THIS IS A COMPILE TEST ONLY - NO BEHAVIOR IS VERIFIED! ***
 * ************************************************************************
 *
 * @author Kurt Mammen
 * @version CPE 102 Lab Quiz 3
 */
import java.io.*;
import java.util.*;

public class BoxCompileTest
{
   public static void main(String[] args)
   {
      Box b = new Box(1.5, 3.0, 4.5, 9.0);
      Box c = new Box(1.5, 3.0, 4.5, 9.0);
      System.out.println(b.equals(c));

      b.getLength();
      b.getWidth();
      b.getHeight();
      b.getWeight();
      b.getVolume();
      b.getMaxDimension();
      b.equals(new Object());

      try
      {
         ArrayList<Box> alist = Box.read("SomeFile");
         System.out.println(alist.get(0).getLength() + ".." + alist.get(0).getVolume() + ".." + alist.get(0).getMaxDimension());
      }
      catch(FileNotFoundException e)
      {
      }
      catch(EOFException e)
      {
      }
      catch(IOException e)
      {
      }
      
      ArrayList<Box> blist = new ArrayList<Box>();
      blist.add(b);

      try
      {
         Box.write("SomeFile", blist);
      }
      catch(FileNotFoundException e)
      {
      }
      catch(IOException e)
      {
      }
   }
}
