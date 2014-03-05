import java.util.ArrayList;

public class ArrayListFun {
   private ArrayList<String> strList;
   private ArrayList<Integer> intList;
   private ArrayList<Double> dblList;
   private ArrayList<Boolean> boolList;
   
   public ArrayListFun() {
      strList = new ArrayList<String>();
      intList = new ArrayList<Integer>();
      dblList = new ArrayList<Double>();
      boolList = new ArrayList<Boolean>();
   }
   
   public void add(String input) {
      strList.add(input);
   }
   
   public void add(int input) {
      intList.add(input);
   }
   
   public void add(double input) {
      dblList.add(input);
   }
   
   public void add(boolean input) {
      boolList.add(input);
   }
   
   public int minimumInt() {
      int minimum = intList.get(0);
      
      for(int i = 0; i < intList.size(); i ++) {
         if(minimum > intList.get(i)) {
            minimum = intList.get(i);
         }
      }
      
      return minimum;
   }
   
   public double averageDouble() {
      double total = 0;
      
      for(int i = 0; i < dblList.size(); i ++) {
         total += dblList.get(i);
      }
      
      return total / dblList.size();
   }
   
   public int numberOfTrues() {
      int total = 0;
      
      for(int i = 0; i < boolList.size(); i ++) {
         if(boolList.get(i)) {
            total ++;
         }
      }
      
      return total;
   }
   
   public int numberOfStrings() {
      return strList.size();
   }
}