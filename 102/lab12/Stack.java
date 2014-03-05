import java.util.*;

public class Stack<E> {
   private LinkedList<E> list;
   
   public Stack() {
      list = new LinkedList<E>();
   }
   
   public void push(E data) {
      list.add(data);
   }
   
   public E pop() {
      return list.remove(list.size()-1);
   }
}