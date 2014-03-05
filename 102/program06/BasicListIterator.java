import java.util.*;

public interface BasicListIterator<E> extends Iterator<E> {
   public boolean hasPrevious();
   public E previous() throws NoSuchElementException;
}