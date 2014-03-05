import java.util.ArrayList;
import java.util.Iterator;

public class TestDriver {
   public static void main(String[] args) {
      ArrayList<Object> list = DataReader.read("file.bin");
      
      for(Object obj : list) {
         System.out.println(obj);
      }
      
      DataReader.write("file2.bin", list);
   }
}