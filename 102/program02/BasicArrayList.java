import java.lang.*;
import java.io.*;
import java.util.*;

public class BasicArrayList implements BasicList {
   public static final int DEFAULT_CAPACITY = 10;
   private Object[] list;
   private int logicalSize;
   
   public BasicArrayList() {
      this(DEFAULT_CAPACITY);
   }
   
   public BasicArrayList(int capacity) {
      if(capacity > DEFAULT_CAPACITY) {
         list = new Object[capacity];
      }
      else {
         list = new Object[DEFAULT_CAPACITY];
      }
      logicalSize = 0;
   }

   public void add(int index, Object element) throws IndexOutOfBoundsException {
      if(index > logicalSize || index < 0) {        // Trying to add too far forward
         throw new IndexOutOfBoundsException();
      }
      if(list.length == logicalSize) { // Need to expand Array
         Object[] nList = new Object[logicalSize * 2];
         for(int i = 0; i < list.length; i ++) {
            nList[i] = list[i];
         }
         list = nList;
      }
      
      if(index != logicalSize) {
         // Need to move code down
         for(int i = list.length-1; i > index; i --) {
            list[i] = list[i-1];
         }
      }
      // All ready to add
      list[index] = element;
      logicalSize++;
   }
   
   public void add(Object element) {
      try {
         add(logicalSize, element);
      }
      catch(IndexOutOfBoundsException e) {
         // Doesn't matter, can't happen
      }
   }
   
   public void clear() {
      list = new Object[DEFAULT_CAPACITY];
      logicalSize = 0;
   }
   
   public boolean contains(Object element) {
      for(int i = 0; i < list.length; i ++) {
         if(element.equals(list[i])) {
            return true;
         }
      }
      return false;
   }
   
   public Object get(int index) throws IndexOutOfBoundsException {
      if(index >= logicalSize || index < 0) {
         throw new IndexOutOfBoundsException();
      }
      return list[index];
   }
   
   public int indexOf(Object element) throws NoSuchElementException {
      for(int i = 0; i < list.length; i ++) {
         if(element.equals(list[i])) {
            return i;
         }
      }
      throw new NoSuchElementException();
   }
   
   public Object remove(int index) throws IndexOutOfBoundsException {
      if(index >= logicalSize || index < 0) {
         throw new IndexOutOfBoundsException();
      }
      Object oldObj = list[index];
      for(int i = index; i < logicalSize-1; i ++) {
         list[i] = list[i+1];
      }
      logicalSize --;
      
      return oldObj;
   }
   
   public Object set(int index, Object element) throws IndexOutOfBoundsException {
      if(index >= logicalSize || index < 0) {
         throw new IndexOutOfBoundsException();
      }
      Object oldObj = list[index];
      
      list[index] = element;
      
      return oldObj;
   }
   
   public int size() {
      return logicalSize;
   }
   
   public void trimToSize() {
      Object[] nList;
      if(logicalSize > DEFAULT_CAPACITY) {
         nList = new Object[logicalSize];
         for(int i = 0; i < logicalSize; i ++) {
            nList[i] = list[i];
         }
      }
      else {
         nList = new Object[DEFAULT_CAPACITY];
         for(int i = 0; i < DEFAULT_CAPACITY; i ++) {
            nList[i] = list[i];
         }
      }
      list = nList;
   }
   
   public int capacity() {
      return list.length;
   }
   
   public Object[] unusualMethodForTestDriverOnly() {
      return list;
   }
}