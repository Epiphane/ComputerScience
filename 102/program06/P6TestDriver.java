/*
 * Program Test Driver - Doubly LinkedList plus iterators,
 * O(1) performance required by some methods.
 *
 * @author Kurt Mammen
 * @version 02/28/2013 - Added null tests for contains and indexOf 
 * @version 11/09/2011 - Added null tests, i.e, adding nulls to list 
 * @version 02/26/2011
 *             - Made BasicLinkedList Iterable
 *             - Added indexOf method to BasicList interface
 * @version 11/06/2010 - Original Revision
 */
import java.lang.reflect.*;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.NoSuchElementException;

public class P6TestDriver
{
   private static final String RESULTS_FOR = "Results for Program 6";

   public static void main(String[] args) throws ClassNotFoundException
   {
      boolean pass = true;
      
      printHeader(args);

      pass &= testBasicListArch();
      pass &= testBasicLinkedListArch();
      pass &= testBasicListIteratorArch();
      
      System.out.println();
      
      pass &= testEmptyList();
      pass &= testListOfOne();
      pass &= testListOfTwo();
      pass &= testListOfThree();
      pass &= testAddForOrderOnePerformance();
      pass &= testSizeForOrderOnePerformance();
      pass &= testClearForOrderOnePerformance();
      pass &= testAddClearRemove();
      pass &= testAddAtIndex();
      pass &= testSet();
      pass &= testContains();
      pass &= testNulls();

      System.out.println();
      
      pass &= testBasicListIterator();
      pass &= testBasicListIteratorForOrderOnePerformance();

      printResults(pass);
      
      if (!pass)
      {
         System.exit(-1);
      }
   }
   
   private static boolean testBasicListArch() throws ClassNotFoundException
   {
      boolean pass = true;
      int cnt;
      Class cl;
      Class[] temp;

      System.out.println("BasicList architecture tests...");

      cl = BasicList.class;
     
      pass &= test(cl.isInterface(), "BasicList is not an interface");

      temp = cl.getInterfaces();

      pass &= test(temp.length == 0, "Expected 0 extended interfaces, found " + temp.length);

      String[] names = {"add", "add", "clear", "contains", "get",
                        "indexOf", "remove", "set", "size"};
      cnt = cl.getDeclaredMethods().length;

      pass &= test(cnt == names.length,
                 "Incorrect number methods, expected " + names.length + ", found " + cnt);
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.INTERFACE, names),
                   "Above method(s) were not specified");
                 
      cnt = cl.getDeclaredFields().length;

      pass &= test(cnt == 0, "Data fields declared, none expected");
      
      return pass;
   }

   private static boolean testBasicLinkedListArch() throws ClassNotFoundException
   {
      boolean pass = true;
      int cnt;
      Class cl;
      Class[] temp;

      System.out.println("BasicLinkedList architecture tests...");

      cl = BasicLinkedList.class;
      
      pass &= test(cl.getSuperclass() == Class.forName("java.lang.Object"),
                   "Class extends something other than Object");

      temp = cl.getInterfaces();
      pass &= test(temp.length == 2,
                   "Expected 2 interfaces, found " + temp.length);

      pass &= test(cl.getConstructors().length == 1,
                   "Expected 1 constructor, found "
                   + cl.getConstructors().length);

      String[] names = {"add", "add", "get", "size", "clear", "contains",
                        "remove", "set", "indexOf", "iterator",
                        "basicListIterator"};

      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);     
      pass &= test(cnt == names.length, "Incorrect number of public methods");
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names),
                   "Above method(s) were not specified");
                 
      cnt = cl.getFields().length;
      pass &= test(cnt == 0, "public instance fields declared, none expected");
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, "protected instance fields declared, none expected");
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt <= 3, cnt + " private instance fields declared, 3 or fewer expected");
      
      // Count and test number of of PACKAGE fields
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, "package instance fields declared, none expected");

      pass &= test(checkInstanceVars(cl.getDeclaredFields()),
                   "Disallowed instance variable type used"); 

      return pass;
   }

   private static boolean testBasicListIteratorArch() throws ClassNotFoundException
   {
      boolean pass = true;
      int cnt;
      Class cl;
      Class[] temp;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();

      System.out.println("BasicListIterator architecture tests...");

      cl = list.basicListIterator().getClass();
      
      pass &= test(cl.getSuperclass() == Class.forName("java.lang.Object"),
       "Class extends something other than Object");
      pass &= test(cl.getInterfaces().length == 1,
       "ListIterator implements incorrect number of interfaces");
      pass &= test(cl.getInterfaces()[0].getName().equals("BasicListIterator"),
       "listIterator() returns incorrect type");
      pass &= test(cl.getInterfaces()[0].getInterfaces().length == 1,
       "ListIterator extends incorrect number of interfaces");
      pass &= test(cl.getInterfaces()[0].getInterfaces()[0].getName().equals("java.util.Iterator"),
       "ListIterator extends incorrect interface");
      temp = cl.getInterfaces();
               
      pass &= test(cl.getConstructors().length <= 1, "Incorrect number of constructors");
      String[] names = {"hasNext", "next", "hasPrevious", "previous", "remove"};

      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);     
      pass &= test(cnt == names.length, "Incorrect number of public methods");
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names),
                   "Unspecified method name(s)");
      
      cnt = cl.getFields().length;
      pass &= test(cnt == 0, "public instance fields declared");
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, "Protected instance fields declared");
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt <= 3, "Too many instance fields declared");
      
      // Count and test number of of PACKAGE fields
      // NOTE: "this" is a package variable for inner classes - so cnt == 1 expected.
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 1, "package instance fields declared");

      return pass;
   }
   
   private static boolean testEmptyList()
   {
      System.out.println("Testing an empty BasicLinkedList...");  

      boolean pass = true;
      boolean caught = false;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();
    
      try
      {
         caught = false;
         list.get(0);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "get(0)");

      try
      {
         caught = false;
         list.get(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "get(-99)");
      pass &= test(list.size() == 0, "size()");

      try
      {
         caught = false;
         list.indexOf(5);
      }
      catch(NoSuchElementException e)
      {
         caught = true;
      }

      pass &= test(caught, "indexOf(5) - expected exception, found index");

      try
      {
         caught = false;
         list.remove(0);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "remove(0)");

      try
      {
         caught = false;
         list.remove(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "remove(-99)");
      
      return pass;
   }
   
   private static boolean testListOfOne()
   {
      System.out.println("Testing an BasicLinkedList of one element...");  

      boolean pass = true;
      boolean caught = false;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();
      
      list.add(new Integer(99));
      
      try
      {
         list.get(1);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "get(1)");

      try
      {
         list.get(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "get(-99)");
      
      pass &= test(list.get(0) == 99, "get(0)");
      pass &= test(list.size() == 1, "size()");

      try
      {
         caught = false;
         list.indexOf(5);
      }
      catch(NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "indexOf(5) - expected excetion, found index");

      int index = -1;
      try
      {
         caught = false;
         index = list.indexOf(new Integer(99));
      }
      catch(NoSuchElementException e)
      {
         caught = true;
      }

      pass &= test(!caught, "indexOf(new Integer(99)) - expected index, found exception");
      pass &= test(index == 0, "indexOf(new Integer(99) - expected 0, found exception");

      try
      {
         caught = false;
         list.remove(1);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "remove(1)");

      try
      {
         caught = false;
         list.remove(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "remove(-99)");
      
      return pass;
   }

   private static boolean testListOfTwo()
   {
      System.out.println("Testing an BasicLinkedList of two elements...");

      boolean pass = true;
      boolean caught = false;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();
      
      list.add(new Integer(99));
      list.add(new Integer(1111));
      
      try
      {
         list.get(2);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "get(2) did not throw IndexOutOfBoundsException");

      try
      {
         list.get(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "get(-99) did not throw IndexOutOfBoundsException");
      
      pass &= test(list.get(0) == 99, "get(0)");
      pass &= test(list.get(1) == 1111, "get(1)");
      pass &= test(list.size() == 2, "size()");
      
      try
      {
         caught = false;
         list.indexOf(5);
      }
      catch(java.util.NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "indexOf(5) - expected exception, found index");

      int index = -1;
      try
      {
         caught = false;
         index = list.indexOf(new Integer(99));
      }
      catch(java.util.NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(!caught, "indexOf(new Integer(99)) (value in list)");
      pass &= test(index == 0, "indexOf(new Integer(99)) (value in list)");

      index = -1;
      try
      {
         caught = false;
         index = list.indexOf(new Integer(1111));
      }
      catch(java.util.NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(!caught, "indexOf(new Integer(1111)) (value in list)");
      pass &= test(index == 1, "indexOf(new Integer(1111) (value in list)");
      
      try
      {
         caught = false;
         list.remove(2);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "remove(2)");

      try
      {
         caught = false;
         list.remove(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "remove(-99)");
      
      return pass;
   }
   
   private static boolean testListOfThree()
   {
      System.out.println("Testing an BasicLinkedList of three elements...");

      boolean pass = true;
      boolean caught = false;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();
      
      list.add(new Integer(99));
      list.add(new Integer(1111));
      list.add(new Integer(-777));
      
      try
      {
         list.get(3);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "get(3)");

      try
      {
         list.get(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "get(-99)");
      
      pass &= test(list.get(0) == 99, "get(0)");
      pass &= test(list.get(1) == 1111, "get(1)");
      pass &= test(list.get(2) == -777, "get(2)");
      pass &= test(list.size() == 3, "size()");
      
      try
      {
         caught = false;
         list.indexOf(new Integer(5));
      }
      catch(java.util.NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "indexOf(new Integer(5)) (value not in list)");

      int index = -1;
      try
      {
         caught = false;
         index = list.indexOf(new Integer(1111));
      }
      catch(java.util.NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(!caught, "indexOf(new Integer(1111)) (value in list)");
      pass &= test(index == 1, "indexOf(new Integer(1111) (value in list)");

      index = -1;
      try
      {
         caught = false;
         index = list.indexOf(new Integer(99));
      }
      catch(java.util.NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(!caught, "indexOf(new Integer(99)) (value in list)");
      pass &= test(index == 0, "indexOf(new Integer(99) (value in list)");

      index = -1;
      try
      {
         caught = false;
         index = list.indexOf(new Integer(-777));
      }
      catch(java.util.NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(!caught, "indexOf(new Integer(-777)) (value in list)");
      pass &= test(index == 2, "indexOf(new Integer(-777) (value in list)");

      try
      {
         caught = false;
         list.remove(3);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "remove(3)");

      try
      {
         caught = false;
         list.remove(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "remove(-99)");
      
      return pass;
   }
   
   private static boolean testAddForOrderOnePerformance()
   {
      System.out.println("Testing add() for O(1) performance...");
      System.out.println("   If running from console <cntl>-C will terminate");
      boolean pass = true;  
      
      BasicLinkedList<Long> ll = new BasicLinkedList<Long>();
      
      for (int i = 0; i < 100000; i++)
      {
         ll.add((long)i);
      }
   
      long start = System.nanoTime();
      ll.add((long)-99);
      double time = System.nanoTime() - start;

      //System.out.println("Expect < 800000, found " + time);

      pass &= test(time < 800000, 
                   "Performance does not appear to be O(1)\n"
                 + "Try running the test driver on VOGON again"); 

      return pass;
   }

   private static boolean testSizeForOrderOnePerformance()
   {
      System.out.println("Testing size() for O(1) performance...");
      System.out.println("   If running from console <cntl>-C will terminate");
      boolean pass = true;  
      double sum = 0;
      
      BasicLinkedList<Long> ll = new BasicLinkedList<Long>();
      for (int i = 0; i < 200000; i++)
      {
         ll.add((long)i);
      }

      long start = System.nanoTime();
      ll.size();
      double time = System.nanoTime() - start;

      //System.out.println("Expect < 35000, found " + time);

      pass &= test(time < 35000,
                   "Performance does not appear to be O(1)\n"
                 + "Try running the test driver on VOGON again"); 

      return pass;
   }
  
   private static boolean testClearForOrderOnePerformance()
   {
      System.out.println("Testing clear() for O(1) performance...");
      System.out.println("   No loops and don't construct a new head object!");
      System.out.println("   If running from console <cntl>-C will terminate");
      boolean pass = true;  
      double sum = 0;
      
      BasicLinkedList<Long> ll = new BasicLinkedList<Long>();
      for (int i = 0; i < 200000; i++)
      {
         ll.add((long)i);
      }

      long start = System.nanoTime();
      ll.clear();
      double time = System.nanoTime() - start;

      //System.out.println("Expect < 22500, found " + time);

      pass &= test(time < 22500,
                   "Performance does not appear to be O(1)\n"
                 + "Try running the test driver on VOGON again"); 

      return pass;
   }
 
   
   private static boolean testAddClearRemove()
   {
      System.out.println("Testing add(int), clear(), and remove(int)...");
      boolean pass = true;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();
      
      pass &= test(list.size() == 0, "size()");
      
      list.clear();
      pass &= test(list.size() == 0, "size() after clear()");
      
      // Add some in order...
      int[] ints = new int[] {99, 88, 77, 66, 55};
      
      for (int i = 0; i < ints.length; i++)
      {
         list.add(ints[i]);
         pass &= test(list.size() == i + 1, "size() while adding elements");

         for(int j = 0; j < list.size(); j++)
         {
            pass &= test(list.get(j) == ints[j], "get(int)");
         }
      }
      
      list.clear();
      pass &= test(list.size() == 0, "size() after clear()");
      
      boolean caught = false;
      
      try
      {
         list.get(0);
      }
      catch (IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "get(0) after clear()");

      // Add some in reverse order...
      for (int i = ints.length - 1; i > -1; i--)
      {
         list.add(ints[i]);
         
         pass &= test(list.size() == ints.length - i, "size() while adding elements");
         int k = ints.length - list.size();
         for(int j = 0; j < list.size(); j++)
         {
            pass &= test(list.get(j) == ints[ints.length - 1 - j], "get(int)");
         }
      }
      
      // Remove last...
      pass &= test(list.remove(4) == 99, "remove(int) - last one");
      pass &= test(list.size() == 4, "size() - after remove");
      
      for (int i = 0; i < ints.length - 1; i++)
      {
         pass &= test(list.get(i) == ints[ints.length - 1 - i], "get(int) after remove");
      }
      
      // Remove first...
      pass &= test(list.remove(0) == 55, "remove(int) - first one");
      pass &= test(list.size() == 3, "size() - after remove");
      
      for (int i = 0; i < ints.length - 2; i++)
      {
         pass &= test(list.get(i) == ints[ints.length - 2 - i], "get(int) after remove");
      }
      
      // Remove middle...
      pass &= test(list.remove(1) == 77, "remove(int) - middle value");
      pass &= test(list.size() == 2, "size() - after remove");
      pass &= test(list.get(0) == 66, "get(int) after remove");
      pass &= test(list.get(1) == 88, "get(int) after remove");
      
      // Remove all
      pass &= test(list.remove(0) == 66, "remove(0) - non-empty list");
      pass &= test(list.size() == 1, "size() after remove()");

      pass &= test(list.remove(0) == 88, "remove(0) - non-empty list");
      pass &= test(list.size() == 0, "size() after remove()");

      list.add(9999);
      pass &= test(list.size() == 1, "size() after add(E)");
      pass &= test(list.get(0) == 9999, "get(int) after remove(int) all");
      
      return pass;
   }

   private static boolean testContains()
   {
      System.out.println("Testing contains(E)...");
      boolean pass = true;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();
      
      // Add some in order...
      int[] ints = new int[] {99, 88, 77, 66, 55};
      
      for (int i = 0; i < ints.length; i++)
      {
         list.add(ints[i]);
         
         for (int j = 0; j < list.size(); j++)
         {
            pass &= test(list.contains(ints[j]), "contains(E) - for a value in the list");
         }
      }

      pass &= test(!list.contains(-1), "contains(E) - for a value not in the list");
      
      return pass;
   }

   private static boolean testAddAtIndex()
   {
      System.out.println("Testing add(int index, E element)...");
      boolean pass = true;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();
      
      boolean caught = false;
      try
      {
         list.add(-89, 7);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "Did not throw IndexOutOfBoundsException for index -89");
      
      caught = false;
      try
      {
         list.add(1, 7);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "Did not throw IndexOutOfBoundsException for index 1");

      int[] ints = new int[] {1, 10, 15, 20};
     
      list.add(0, 10);
      pass &= test(list.get(0) == 10, "add(0,10) - expected 10, found " + list.get(0));
      
      list.add(0, 1);
      pass &= test(list.get(0) == 1, "add(0,1) - expected 1, found " + list.get(0));
      pass &= test(list.get(1) == 10, "add(0,1) - expected 10, found " + list.get(1));  
      
      list.add(2, 20);
      pass &= test(list.get(0) == 1, "add(2,20) - expected 1, found " + list.get(0));
      pass &= test(list.get(1) == 10, "add(2,20) - expected 10, found " + list.get(1));
      pass &= test(list.get(2) == 20, "add(2,20) - expected 20, found " + list.get(2));
   
      list.add(2, 15);
      pass &= test(list.get(0) == 1, "add(2,15) - expected 1, found " + list.get(0));
      pass &= test(list.get(1) == 10, "add(2,15) - expected 10, found " + list.get(1));
      pass &= test(list.get(2) == 15, "add(2,15) - expected 15, found " + list.get(2));
      pass &= test(list.get(3) == 20, "add(2,15) - expected 20, found " + list.get(3));
      
      for (int i = 0; i < ints.length; i++)
      {
         pass &= test(list.get(i) == ints[i], "add(" + i + ", " + ints[i] + ")");
      }
     
      return pass;
   }

   private static boolean testSet()
   {
      System.out.println("Testing set(int index, E element)...");
      boolean pass = true;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();
      
      boolean caught = false;
      try
      {
         list.set(0, 5);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "Did not throw IndexOutOfBoundsException for set(0, 5)");
      
      list.add(99);
      double returnedValue = list.set(0, -111);
      pass &= test(returnedValue == 99, "Incorrect return value for set(0, -111)");
      pass &= test(list.get(0) == -111, "Incorrect get(0) value after set(0, -111)");
      
      caught = false;
      try
      {
         list.set(1, 8);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
    
      pass &= test(caught, "Did not throw IndexOutOfBoundsException for set(1, 8");
           
      list.add(-55);
      list.add(-22);
      
      list.set(0, 11);
      pass &= test(list.get(0) == 11, "set(0, 11)");
      
      list.set(1, 22);
      pass &= test(list.get(1) == 22, "set(0, 22)");
      
      list.set(2, 33);
      pass &= test(list.get(2) == 33, "set(0, 33)");
      
      return pass;
   }

   private static boolean testBasicListIterator()
   {
      System.out.println("Testing BasicListIterator...");
      boolean pass = true;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();
      int[] ints = new int[] {99, 88, 77, 66, 55};
      
      BasicListIterator<Integer> iter;
      
      // Test that remove method of Iterator interface throws as specified...
      boolean caught = false;
      
      iter = list.basicListIterator();      
      try
      {
         iter.remove();
      }
      catch(UnsupportedOperationException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.remove() not throwing UnsupportedOperationException");
         
      // Test empty list...      
      pass &= test(!iter.hasNext(), "ListIterator.hasNext() on empty list");
      pass &= test(!iter.hasPrevious(), "ListIterator.hasPrevious() on empty list");
      
      caught = false;
      try
      {
         iter.next();
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.next() not throwing when it should");

      caught = false;
      try
      {
         iter.previous();
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.previous() not throwing when it should");

      // Test list of one...
      list.add(ints[0]);
      iter = list.basicListIterator();

      pass &= test(iter.hasNext(), "ListIterator.hasNext() on list of one");
      pass &= test(!iter.hasPrevious(), "ListIterator.hasPrevious() on list of one");
      
      caught = false;
      try
      {
         pass &= test(iter.next() == ints[0], "ListIterator.hasNext() on non-empty list");
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(!caught, "ListIterator.next() throwing when it should not");

      caught = false;
      try
      {
         pass &= test(iter.previous() == ints[0], "ListIterator.hasPrevious() on non-empty list");
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(!caught, "ListIterator.previous() throwing when it should not");

      caught = false;
      try
      {
         iter.next();
         iter.next();
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.next() not throwing when it should");

      // Test list of two...
      list.add(ints[1]);
      iter = list.basicListIterator();
      
      int i = 0;
      while(iter.hasNext())
      {
         pass &= test(iter.next() == ints[i++], "ListIterator not working correctly");
      }
      
      caught = false;
      try
      {
         iter.next();
       }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.next() not throwing when it should");

      while(iter.hasPrevious())
      {
         pass &= test(iter.previous() == ints[--i], "ListIterator not working correctly");
      }

      caught = false;
      try
      {
         iter.previous();
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.previous() not throwing when it should");

      // Test list of many...
      list.add(ints[2]);
      list.add(ints[3]);
      list.add(ints[4]);
      iter = list.basicListIterator();

      i = 0;
      while(iter.hasNext())
      {
         pass &= test(iter.next() == ints[i++], "ListIterator not working correctly");
      }
      
      caught = false;
      try
      {
         iter.next();
       }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.next() not throwing when it should");

      while(iter.hasPrevious())
      {
         pass &= test(iter.previous() == ints[--i], "ListIterator not working correctly");
      }

      caught = false;
      try
      {
         iter.previous();
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.previous() not throwing when it should");
      
      // Remove some elements and iterate over remaining ones...
      pass &= test(list.remove(2) == ints[2], "remove(int) after iteration");
      pass &= test(list.remove(3) == ints[4], "remove(int) after iteration");
      pass &= test(list.remove(0) == ints[0], "remove(int) after iteration");
      
      iter = list.basicListIterator();
      
      pass &= test(iter.hasNext(), "hasNext() after remove(int)");
      pass &= test(iter.next() == ints[1], "next() after remove(int)");
      
      pass &= test(iter.hasNext(), "hasNext() after remove(int)");
      pass &= test(iter.next() == ints[3], "next() after remove(int)");

      // Add some elements and iterate over list...
      list.add(ints[2]);
      list.add(ints[4]);
      list.add(ints[0]);
      iter = list.basicListIterator();
      
      pass &= test(list.size() == 5,"size() after remove-add");
      
      iter = list.basicListIterator();
      i = 0;
      
      int expected[] = {88, 66, 77, 55, 99};
      
      while(iter.hasNext())
      {
         pass &= test(iter.next() == expected[i++], "ListIterator not working correctly");
      }
      
      caught = false;
      try
      {
         iter.next();
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.next() not throwing when it should");

      while(iter.hasPrevious())
      {
         pass &= test(iter.previous() == expected[--i], "ListIterator not working correctly");
      }

      caught = false;
      try
      {
         iter.previous();
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "ListIterator.previous() not throwing when it should");

      // Remove all and iterate...
      list.remove(4);
      list.remove(1);
      list.remove(2);
      list.remove(0);
      list.remove(0);
      
      pass &= test(list.size() == 0, "size() after remove(int)");
      
      iter = list.basicListIterator();
      
      pass &= test(!iter.hasNext(), "ListIterator.hasNext() after remove(int)");
      pass &= test(!iter.hasPrevious(), "ListIterator.hasPrevious() after remove(int)");
      
      // Add some and iterate...
      for (i = ints.length - 1; i > -1; i--)
      {
         list.add(ints[i]);
      }
      
      iter = list.basicListIterator();

      i = 4;
      while(iter.hasNext())
      {
         pass &= test(iter.next() == ints[i--], "ListIterator.next() after add(E)");
      }

      pass &= test(iter.hasPrevious(), "ListIterator.hasPrevious() after next()");

      // Clear and iterate...
      list.clear();

      iter = list.basicListIterator();
      
      pass &= test(!iter.hasNext(), "ListIterator.hasNext() after clear()");
      pass &= test(!iter.hasPrevious(), "ListIterator.hasPrevious() after clear()");
      
      return pass;
   }

   private static boolean testBasicListIteratorForOrderOnePerformance()
   {
      System.out.println("Testing BasicListIterator() for O(1) performance...");
      System.out.println("   If running from console <cntl>-C will terminate");
      boolean pass = true;  
      
      BasicLinkedList<Long> ll = new BasicLinkedList<Long>();
      
      for (int i = 0; i < 1000; i++)
      {
         ll.add((long)i);
      }
   
      double temp = 0;
      BasicListIterator<Long> it = ll.basicListIterator();
      
      long start = System.nanoTime();     
      while(it.hasNext())
      {
         temp += it.next();
      }
      double time = System.nanoTime() - start;
      
      pass &= test(time < 3200000, 
                   "next() does not appear to be O(1), try running the test driver again"); 

      start = System.nanoTime();     
      while(it.hasPrevious())
      {
         temp += it.previous();
      }
      time = System.nanoTime() - start;
      
      pass &= test(time < 3200000, 
                   "previous() does not appear to be O(1), try running the test driver again"); 

      return pass;
   }

   private static boolean testNulls()
   {
      System.out.println("Testing BasicLinkedList with null elements...");
      boolean pass = true;
      BasicLinkedList<Integer> list = new BasicLinkedList<Integer>();

      for (int i = 0; i < 20; i++)
      {
         if (i % 2 == 0)
         {
            list.add(null);
         }
         else
         {
            list.add(i);
         }

         for (int j = i; j < i; j++)
         {
            if (j % 2 == 0)
            {
               pass &= test(list.get(j) == null, "get() = expected null element");
            }
            else
            {
               pass &= test(list.get(j) == null, "get() - expected " + j + " element");
            }
         }
      }

      BasicListIterator<Integer> it = list.basicListIterator();

      for (int i = 0; i < list.size(); i++)
      {
         Integer value = it.next();

         if (i % 2 == 0)
         {
            pass &= test(value == null, "BasicListIterator.next = expected null element");
         }
         else
         {
            pass &= test(value == i, "BasicListIterator.next = expected " + i);
         }
      }

      for (int i = 19; i < -1; i--)
      {
         Integer value = it.previous();

         if (i % 2 == 0)
         {
            pass &= test(value == null, "BasicListIterator.previous = expected null element");
         }
         else
         {
            pass &= test(value == i, "BasicListIterator.previous = expected " + i);
         }
      }

      pass &= test(list.remove(18) == null, "remove() - expected null element");
      pass &= test(list.remove(18) == 19, "remove() - expected 19");

      for (int i = 0; i < 18; i++)
      {
         if (i % 2 == 0)
         {
            pass &= test(list.remove(0) == null, "remove() - expected null element");
         }
         else
         {
            pass &= test(list.remove(0) == i, "remove() - expected " + i);
         }
      }

      // Test contains and indexOf...
      BasicLinkedList<String> slist = new BasicLinkedList<String>();

      slist.add("Hello");
      slist.add("World");

      pass &= test(!slist.contains(null), "contains(null) when list does not contain null");

      boolean caught = false;

      try
      {
         slist.indexOf(null);
      }
      catch (NoSuchElementException e)
      {
         caught = true;
      }

      pass &= test(caught, "indexOf(null) when list does not contain null");

      slist.add(null);
      slist.add("peanut");
      slist.add("butter");

      pass &= test(slist.contains(null), "contains(null) when list contains null");
      
      pass &= test(slist.indexOf(null) == 2, "indexOf(null) when list contains null");

      return pass;
   }
   
   private static void printHeader(String[] args)
   {
      if (args.length == 1)
      {
         System.out.println(args[0]);
      }
      
      System.out.println(RESULTS_FOR + "\n");
   }

   private static void printResults(boolean pass)
   {
      String msg;
      
      if(pass)
      {
         msg = "\nCongratulations, you MAY HAVE passed all the tests!\n\n"
            + "The tests checking for O(1) performance are designed to work\n"
            + "correctly on the machine(s) you are required to test and\n"
            + "hand in on. Be sure you test your solution on that/those\n"
            + "machine(s) to avoid the possiblity your system is so blazingly\n"
            + "fast it covered up performance problems in your solution. Make\n"
            + "sure your solution passes several times before considering it\n"
            + "correct.\n\n"
            + "If/when your code consistently pass all of the tests your\n"
            + "grade will be based on when you turn in your functionally\n"
            + "correct solution and any deductions for the quality of your\n"
            + "implementation. Quality is based on, but not limited to,\n"
            + "coding style, documentation requirements, compiler warnings\n"
            + "and the efficiency and elegance of your code.\n";
      }
      else
      {
         msg = "\nNot done yet - you failed one or more tests!\n\n"
            + "If you are failing because of the tests checking for O(1)\n"
            + "performance make sure you are running on one of the systems\n"
            + "you are required to test and hand in on to avoid the\n"
            + "possibility that the reason you are failing is simply\n"
            + "that your system is too slow.  If you are running on one of\n"
            + "those systems and your code is failing consistently, you will\n"
            + "need to fix your code.\n";
      }
      
      System.out.print(msg);       
   }
   
   private static int countModifiers(Field[] fields, int modifier)
   {
      int count = 0;
      
      for (Field f : fields)
      {
         if (f.getModifiers() == modifier)
         {
            count++;
         }
      }
      
      return count;
   }
   
   private static int countModifiers(Method[] methods, int modifier)
   {
      int count = 0;
      
      for (Method m : methods)
      {
         if (m.getModifiers() == modifier)
         {
            count++;
         }
      }
      
      return count;
   }

   private static boolean checkInstanceVars(Field[] fields)
   {
      for (Field f : fields)
      {
         if (f.toString().contains("java."))
         {
            return false;
         }
      }
 
      return true;
   }
   
   private static boolean approx(double a, double b, double epsilon)
   {
      return Math.abs(a - b) < epsilon;
   }
   
   private static boolean verifyNames(Method[] methods, int modifier, String[] names)
   {
      boolean pass = true;
      Arrays.sort(names);
      
      for (Method m : methods)
      {
         if (m.getModifiers() == modifier)
         {
            if (Arrays.binarySearch(names, m.getName()) < 0)
            {
               System.out.print("      Class contains unspecified public ");
               System.out.println("method: " + m.getName());
               pass &= false;
            }
         }
      }
      
      return pass;
   }
         
   private static boolean test(boolean pass, String msg)
   {
      if (!pass)
      {
         (new Throwable("   Failed: " + msg)).printStackTrace();
      }
      
      return pass;
   }
   
   private static int countPackage(Field[] fields)
   {
      int cnt = fields.length
                - countModifiers(fields, Modifier.PRIVATE)
                - countModifiers(fields, Modifier.PROTECTED)
                - countModifiers(fields, Modifier.PUBLIC);

      // Adjust for students that have written assert statment(s) in their code
      // The package field specified below gets added to the .class file when
      // assert statements are present in the source.
      for (Field f : fields)
      {
         int mods = f.getModifiers();
         
         if (Modifier.isStatic(mods)
          && Modifier.isFinal(mods)
          && f.getName().equals("$assertionsDisabled"))
         {
            cnt--;
         }
      }
      
      return cnt;
   } 
}
