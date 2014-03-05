public class Tests {
   public static void main(String[] args) {
      A a = new D();
      B b = new D();
      C c = new C();
      D d = new D();
      E e = new E();

      d.methodD((D)b);
      //d = a;              // Fails to compile - a only implements A, not A and B
      b = new E();
      //b = (D)c;           // Fails to compile - C cannot be cast to D
      //b.methodD(d);       // Fails to compile - B interface has no methodD
      //((D)b).methodD(d);  // Fails to run after b = new E(); in runtime
      a = c;
      b.methodB();
      //b = new B();        // Fails to compile, since B cannot be constructed        
      e.methodB();
   }
}
