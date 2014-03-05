/**
 * @author Thomas Steinke
 * @version Lab Quiz 3
 */

import java.util.ArrayList;
import java.io.*;

public class Box {
   double length, width, height, weight;

   public Box(double length, double width, double height, double weight) {
      this.length = length;
      this.width = width;
      this.height = height;
      this.weight = weight;
   }
   
   public double getLength() {
      return length;
   }
   
   public double getWidth() {
      return width;
   }
   
   public double getHeight() {
      return height;
   }
   
   public double getWeight() {
      return weight;
   }
   
   public double getVolume() {
      return length*width*height;
   }
   
   public double getMaxDimension() {
      if(length > width) {
         if(length > height) {
            return length;
         }
         else {
            return height;
         }
      }
      else {
         if(width > height) {
            return width;
         }
         else {
            return height;
         }
      }
   }
   
   public boolean equals(Object o) {
      if(o == null) {
         return false;
      }
      if(o.getClass() != this.getClass()) {
         return false;
      }
      if(length != ((Box)o).length) {
         return false;
      }
      if(width != ((Box)o).width) {
         return false;
      }
      if(height != ((Box)o).height) {
         return false;
      }
      return weight == ((Box)o).weight;
   }
   
   public static void write(String fileName, ArrayList<Box> boxes) throws IOException {
      DataOutputStream dataStream = new DataOutputStream(new FileOutputStream(fileName));
      for(Box box : boxes) {
         dataStream.writeDouble(box.getLength());
         dataStream.writeDouble(box.getWidth());
         dataStream.writeDouble(box.getHeight());
         dataStream.writeDouble(box.getWeight());
      }
   }
   
   public static ArrayList<Box> read(String fileName) throws IOException {
      ArrayList<Box> listOfBoxes = new ArrayList<Box>();
      DataInputStream dataStream = new DataInputStream(new FileInputStream(fileName));
      try {
         while(true) {
            double length = dataStream.readDouble();
            double width = dataStream.readDouble();
            double height = dataStream.readDouble();
            double weight = dataStream.readDouble();
            
            listOfBoxes.add(new Box(length, width, height, weight));
         }
      }
      catch(EOFException e) {
      }
   
      return listOfBoxes;
         
   }
}