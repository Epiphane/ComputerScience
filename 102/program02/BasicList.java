import java.lang.*;
import java.util.*;

public interface BasicList {
   public void add(int index, Object element) throws java.lang.IndexOutOfBoundsException;
   public void add(Object element);
   public void clear();
   public boolean contains(Object element);
   public Object get(int index) throws java.lang.IndexOutOfBoundsException;
   public int indexOf(Object element) throws NoSuchElementException;
   public Object remove(int index) throws java.lang.IndexOutOfBoundsException;
   public Object set(int index, Object element) throws java.lang.IndexOutOfBoundsException;
   public int size();
}