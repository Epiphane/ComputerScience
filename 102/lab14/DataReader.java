import java.io.*;
import java.util.ArrayList;
import java.util.Iterator;

public class DataReader {
   public static ArrayList<Object> read(String fileName) {
      try {
         DataInputStream dataStream = new DataInputStream(new FileInputStream(fileName));
         
         ArrayList<Object> listOfObjects = new ArrayList<Object>();
         try {
            while(true) {
               Integer amtOfDoubles = dataStream.readInt();
               listOfObjects.add(amtOfDoubles);
               for(int i = 0; i < amtOfDoubles; i ++) {
                  listOfObjects.add(dataStream.readDouble());
               }
            }
         }
         catch(EOFException e) {
         }
         finally {
            return listOfObjects;
         }
      }
      catch(IOException e) {
         e.printStackTrace();
         return null;
      }
   }
   
   public static void write(String fileName, ArrayList<Object> list) throws IllegalArgumentException {
      try {
         DataOutputStream dataStream = new DataOutputStream(new FileOutputStream(fileName));
         
         Iterator<Object> it = list.iterator();
         while(it.hasNext()) {
            int amtOfDoubles = (Integer)it.next();
            dataStream.writeInt(amtOfDoubles);
            for(int i = 0; i < amtOfDoubles; i ++) {
               dataStream.writeDouble((Double)it.next());
            }
         }
      }
      catch(IOException e) {
         throw new IllegalArgumentException();
      }
   }
}