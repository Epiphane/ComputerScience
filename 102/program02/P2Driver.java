public class P2Driver {
   public static void main(String[] args) {
      BasicArrayList list = new BasicArrayList();
      
      if(list.size() != 0) {
         System.out.println("Error creating list: Expected size 0, got "+list.size());
      }
      
      list.add(new Integer(5));
      
   }
   
   public void test(BasicArrayList list, int expectedSize, int expectedCapacity) {
   }
}