import java.util.*;

public interface BasicList<E> {
   public void add(int index, E element) throws java.lang.IndexOutOfBoundsException;
   public void add(E element);
   public void clear();
   public boolean contains(E element);
   public E get(int index) throws java.lang.IndexOutOfBoundsException;
   public int indexOf(E element) throws NoSuchElementException;
   public E remove(int index) throws java.lang.IndexOutOfBoundsException;
   public E set(int index, E element) throws java.lang.IndexOutOfBoundsException;
   public int size();
}