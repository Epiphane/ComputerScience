import java.util.*;
import java.lang.*;

public class BasicLinkedList<E> implements BasicList<E>, java.lang.Iterable<E> {
   private Node head;
   private int size;
   private class Node {
      public E data;
      public Node prev,next;
   }
   
   public class It implements BasicListIterator<E> {
      private Node cursor;
      public It() {
         cursor = head;
      }
      
      public boolean hasNext() {
         return (cursor.next != head);
      }
      
      public E next() throws NoSuchElementException {
         if(cursor.next == head) {
            throw new NoSuchElementException();
         }
         cursor = cursor.next;
         return cursor.data;
      }
      
      public boolean hasPrevious() {
         return (cursor.prev.prev != head);
      }
      
      public E previous() throws NoSuchElementException {
         if(cursor.prev.prev == head) {
            throw new NoSuchElementException();
         }
         cursor = cursor.prev;
         return cursor.next.data;
      }
      
      public void remove() {
         throw new UnsupportedOperationException();
      }
   }
   
   public BasicLinkedList() {
      head = new Node();
      head.next = head.prev = head;
   }

   public void add(int index, E element) throws IndexOutOfBoundsException {
      if(index == size) {
         add(element);
      }
      if(index > size || index < 0) {
          throw new IndexOutOfBoundsException();
       }
       
       Node cursor = head;
       do {
          cursor = cursor.next;
       } while(--index >= 0);
       
       Node n = new Node();
       n.data = element;
       
       n.prev = cursor.prev;
       n.next = cursor;
       cursor.prev.next = n;
       cursor.prev = n;
       
       size ++;
   }
   
   public void add(E element) {
      Node n = new Node();
      n.data = element;
      
      n.prev = head.prev;
      n.next = head;
      head.prev.next = n;
      head.prev = n;
      
      size ++;
   }
   
   public void clear() {
      head.next = head.prev = head;
      size = 0;
   }
   
   public boolean contains(E element) {
      It iterator = new It();
      while(iterator.hasNext()) {
         E data = iterator.next();
         if(data == element || data.equals(element)) {
            return true;
         }
      }
      return false;
   }
   
   public E get(int index) throws IndexOutOfBoundsException {
      if(index >= size || index < 0) {
         throw new IndexOutOfBoundsException();
      }
      
      Node cursor = head;
      do {
         cursor = cursor.next;
      } while(--index >= 0);

      return cursor.data;
   }
   
   public int indexOf(E element) throws NoSuchElementException {
      It iterator = new It();
      int index = -1;
      while(iterator.hasNext()) {
         index ++;
         E data = iterator.next();
         if(data == element || data.equals(element)) {
            return index;
         }
      }
      throw new NoSuchElementException();
   }
   
   public E remove(int index) throws IndexOutOfBoundsException {
      if(index >= size || index < 0) {
          throw new IndexOutOfBoundsException();
       }
      
       Node cursor = head;
       do {
          cursor = cursor.next;
       } while(--index >= 0);
       
       cursor.prev.next = cursor.next;
       cursor.next.prev = cursor.prev;
       
       size --;
       return cursor.data;
   }
   
   public E set(int index, E element) throws IndexOutOfBoundsException {
      if(index >= size || index < 0) {
         throw new IndexOutOfBoundsException();
      }
      
      Node cursor = head;
      do {
         cursor = cursor.next;
      } while(--index >= 0);
      E olddata = cursor.data;
      cursor.data = element;
      
      return olddata;
   }
   
   public int size() {
      return size;
   }
   
   public BasicListIterator<E> basicListIterator() {
      return new It();
   }
   
   public Iterator<E> iterator() {
      return new It();
   }
}
