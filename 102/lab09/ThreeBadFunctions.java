public class ThreeBadFunctions {
   public void nullPointer() {
      String j;
      j = null;
      
      System.out.println(j.equals("Hello World"));
   }
   
   public void classCast() {
      Object a = new Driver();
      
      ((ThreeBadFunctions) a).nullPointer();
   }
   
   public void arrayIndex() {
      int[] arrayOfStuff = new int[] {0,1,2,3};
      
      System.out.println(arrayOfStuff[4]);
   }
   
   public void dumbCaller() throws DumbCallerException {
      throw new DumbCallerException();
   }
   
   public void runtimeProblems()  {
      throw new RandomRuntimeException();
   }
   
   public void doStuff(int methodNum) throws DumbCallerException {
      switch(methodNum) {
      case 1:
         nullPointer();
         break;
      case 2:
         classCast();
         break;
      case 3:
         arrayIndex();
         break;
      case 4:
         dumbCaller();
         break;
      case 5:
         runtimeProblems();
         break;
      }
   }
}