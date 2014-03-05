import java.util.ArrayList;

public class ArrayListFun {
   private ArrayList<Object> myList;
   
   public ArrayListFun() {
      myList = new ArrayList<Object>();
   }
   
   public void add(String input) {
      myList.add(input);
   }
   
   public void add(int input) {
      myList.add(input);
   }
   
   public void add(double input) {
      myList.add(input);
   }
   
   public void add(boolean input) {
      myList.add(input);
   }
   
   public int minimumInt() {
      int minimum = 0;
      boolean minimumInit = false;
      
      for(int i = 0; i < myList.size(); i ++) {
         if(!(myList.get(i) instanceof Integer)) {
            continue;
         }
         if(!minimumInit || minimum > (Integer) myList.get(i)) {
            minimum = (Integer) myList.get(i);
            minimumInit = true;
         }
      }
      
      return minimum;
   }
   
   public double averageDouble() {
      double total = 0;
      int numDoubles = 0;
      
      for(int i = 0; i < myList.size(); i ++) {
         if(myList.get(i) instanceof Double) {
            total += (Double) myList.get(i);
            numDoubles ++;
         }
      }
      
      return total / numDoubles;
   }
   
   public int numberOfTrues() {
      int total = 0;
      
      for(int i = 0; i < myList.size(); i ++) {
         if(myList.get(i) instanceof Boolean && ((Boolean) myList.get(i))) {
            total ++;
         }
      }
      
      return total;
   }
   
   public int numberOfStrings() {
      int total = 0;
      
      for(int i = 0; i < myList.size(); i ++) {
         if(myList.get(i) instanceof String) {
            total ++;
         }
      }
      
      return total;
   }
}