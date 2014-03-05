/**
 * Program Test Driver - BasicArrayList
 *
 * @author Kurt Mammen
 * @version 10/07/12 - Added check that list supports null.
 * @version 01/21/11 - Fixed indexOf test that was not printing message.
 * @version 01/13/11 - Added indexOf to BasicList
 * @version 11/06/10 - Original
 */
import java.lang.reflect.*;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.NoSuchElementException;

public class P2TestDriver
{
   private static final String RESULTS_FOR = "Results for Program 2";

   public static void main(String[] args) throws ClassNotFoundException
   {
      boolean pass = true;
      
      printHeader(args);

      pass &= testBasicListArch();
      pass &= testBasicArrayListArch();
      
      System.out.println();

      /* Test mutating methods (accessors tested indirectly */
      pass &= testAdd();
      pass &= testAddAtIndex();
      pass &= testClear();
      pass &= testRemove();
      pass &= testSet();
      pass &= testTrimToSize();
      pass &= testNullSupport();

      System.out.println("\nThe following methods were tested in the process" );
      System.out.println("of testing the methods listed above:\n");
      System.out.println("   capacity()");
      System.out.println("   contains(Object o)");
      System.out.println("   get(int index)");
      System.out.println("   indexOf(Object o)");
      System.out.println("   size()");
      System.out.println("   unusualMethodForTestDriverOnly()");

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

      pass &= test(temp.length == 0, "Incorrect number of implemented interfaces");

      String[] names = {"add", "add", "clear", "contains", "get", "indexOf",
                        "remove", "set", "size"};
      cnt = cl.getDeclaredMethods().length;

      pass &= test(cnt == names.length,
                 "Incorrect number methods, expected " + names.length + ", found " + cnt);
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.INTERFACE, names),
                   "Above method(s) were not specified");
                 
      cnt = cl.getDeclaredFields().length;

      pass &= test(cnt == 0, "Data fields declared, none expected");
      
      return pass;
   }

   private static boolean testBasicArrayListArch() throws ClassNotFoundException
   {
      boolean pass = true;
      int cnt;
      Class cl;
      Class[] temp;

      System.out.println("BasicArrayList architecture tests...");

      cl = BasicArrayList.class;
      
      pass &= test(cl.getSuperclass() == Class.forName("java.lang.Object"),
              "Class extends something other than Object");

      temp = cl.getInterfaces();
      pass &= test(temp.length == 1,
              "Incorrect number of interfaces, expected 1, found " + temp.length);

      if (temp.length == 1)
      {
         String interfaceName = "BasicList";
         pass &= test(temp[0].getName().equals(interfaceName),
                 "Expected " + interfaceName + " interface, found " + temp[0].getName());
      }

      pass &= test(cl.getConstructors().length == 2, "Incorrect number of constructors");

      String[] names = {"add", "add", "clear", "contains", "get", "indexOf", "remove", "set",
                        "size", "trimToSize", "capacity", "unusualMethodForTestDriverOnly"};

      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);     
      pass &= test(cnt == names.length, "Incorrect number of public methods");
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names),
                   "Above method(s) were not specified");
                 
      cnt = cl.getFields().length;
      pass &= test(cnt == 1, cnt + " public fields declared, 1 expected");
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, "protected instance fields declared, none expected");
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt == 2, cnt + " private instance fields declared, 2 expected");
      
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, "package instance fields declared, none expected");

      pass &= test(checkInstanceVars(cl.getDeclaredFields()),
                   "Disallowed instance variable type used"); 

      return pass;
   }

   private static boolean testAdd()
   {
      System.out.println("Testing add(Object o)...");
      boolean pass = true;
   
      BasicArrayList list = new BasicArrayList();
      Object[] expected = new Object[0];

      pass &= testList(list, expected, 0, BasicArrayList.DEFAULT_CAPACITY);

      list.add(100);
      expected = new Integer[] {100};
      pass &= testList(list, expected, 1, BasicArrayList.DEFAULT_CAPACITY);

      list.add(200);
      expected = new Integer[] {100, 200};
      pass &= testList(list, expected, 2, BasicArrayList.DEFAULT_CAPACITY);

      list.add(300);
      expected = new Integer[] {100, 200, 300};
      pass &= testList(list, expected, 3, BasicArrayList.DEFAULT_CAPACITY);

      for (int i = 3; i < 10; i++)
      {
         int value = (i+1) * 100;
         list.add(value);
      }
      expected = new Integer[] {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
      pass &= testList(list, expected, 10, BasicArrayList.DEFAULT_CAPACITY);
     
      list.add(1100);
      expected = new Integer[] { 100,  200,  300,  400,  500,  600,  700,  800,  900, 1000,
                                1100};
      pass &= testList(list, expected, 11, BasicArrayList.DEFAULT_CAPACITY * 2);
      
      for (int i = 11; i < 20; i++)
      {
         int value = (i+1) * 100;
         list.add(value);
      }
      expected = new Integer[] { 100,  200,  300,  400,  500,  600,  700,  800,  900, 1000,
                                1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};
      pass &= testList(list, expected, 20, BasicArrayList.DEFAULT_CAPACITY * 2);

      list.add(2100);
      expected = new Integer[] { 100,  200,  300,  400,  500,  600,  700,  800,  900, 1000,
                                1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000,
                                2100};
      pass &= testList(list, expected, 21, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      for (int i = 21; i < 40; i++)
      {
         int value = (i+1) * 100;
         list.add(value);
      }
      expected = new Integer[] { 100,  200,  300,  400,  500,  600,  700,  800,  900, 1000,
                                1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000,
                                2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000,
                                3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000};
      pass &= testList(list, expected, 40, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      list.add(4100);
      expected = new Integer[] { 100,  200,  300,  400,  500,  600,  700,  800,  900, 1000,
                                1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000,
                                2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000,
                                3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000,
                                4100};
      pass &= testList(list, expected, 41, BasicArrayList.DEFAULT_CAPACITY * 2 * 2 * 2);

      return pass;
   }

   private static boolean testAddAtIndex()
   {
      System.out.println("Testing add(int index, Object o)...");
      boolean pass = true;
      boolean caught;
   
      BasicArrayList list = new BasicArrayList();
      Object[] expected = new Object[0];

      try
      {
         caught = false;
         list.add(-2, -2.0);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "did not throw expected IndexOutOfBoundsException"); 

      try
      {
         caught = false;
         list.add(1, -2.0);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "did not throw expected IndexOutOfBoundsException"); 

      try
      {
         caught = false;
         list.add(99, -2.0);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "did not throw expected IndexOutOfBoundsException"); 

      list.add(0, 0.2);
      expected = new Double[] {0.2};
      pass &= testList(list, expected, 1, BasicArrayList.DEFAULT_CAPACITY);
      
      list.add(0, 0.0);
      expected = new Double[] {0.0, 0.2};
      pass &= testList(list, expected, 2, BasicArrayList.DEFAULT_CAPACITY);

      list.add(1, 0.1);
      expected = new Double[] {0.0, 0.1, 0.2};
      pass &= testList(list, expected, 3, BasicArrayList.DEFAULT_CAPACITY);

      expected = new Double[] {  0.0,  0.1,  0.2,  0.3,  0.4, -9.1, 22.7, 0.11, -0.7, 99.9,
                                33.3,  9.1,  1.2, 88.8, 2.22, 34.5, 82.8, 2.21, -0.2, 19.9};
      for (int i = 3; i < 20 ; i++)
      {
         list.add(3, expected[22 - i]);
      }
      pass &= testList(list, expected, 20, BasicArrayList.DEFAULT_CAPACITY * 2);

      return pass;
   }

   private static boolean testClear()
   {
      System.out.println("Testing clear()...");
      boolean pass = true;
      Object[] expected;

      BasicArrayList list = new BasicArrayList();
      list.clear();
      expected = new Object[] {};
      pass &= testList(list, expected, 0, BasicArrayList.DEFAULT_CAPACITY);

      list.add(100);
      list.clear();
      pass &= testList(list, expected, 0, BasicArrayList.DEFAULT_CAPACITY);

      expected = new java.awt.Point[200];

      for (int i = 0; i < 200; i++)
      {
         java.awt.Point p = new java.awt.Point(i, -i);
         list.add(p);
         expected[i] = p;
      }

      pass &= testList(list, expected, 200, BasicArrayList.DEFAULT_CAPACITY * 2 * 2 * 2 * 2 * 2);

      list.clear();
      expected = new Object[] {};
      pass &= testList(list, expected, 0, BasicArrayList.DEFAULT_CAPACITY);

      return pass;
   }
   
   private static boolean testRemove()
   {
      System.out.println("Testing remove(int index)...");
      boolean pass = true;
      boolean caught;
      int removed;
      Object[] expected;

      BasicArrayList list = new BasicArrayList();

      try
      {
         caught = false;
         list.remove(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "did not throw expected IndexOutOfBoundsException"); 

      try
      {
         caught = false;
         list.remove(0);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "did not throw expected IndexOutOfBoundsException"); 

      try
      {
         caught = false;
         list.remove(99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "did not throw expected IndexOutOfBoundsException"); 


      // Add one thing and remove it...
      expected = new Integer[] {1};
      list.add(1);
      pass &= testList(list, expected, 1, BasicArrayList.DEFAULT_CAPACITY);
      removed = (Integer)list.remove(0);
      pass &= test(removed == 1, "expected 1, found " + removed);
      expected = new Integer[] {};
      pass &= testList(list, expected, 0, BasicArrayList.DEFAULT_CAPACITY);

      // Add two things and remove the first one...
      expected = new Integer[] {1, 2};
      list.add(1);
      list.add(2);
      pass &= testList(list, expected, 2, BasicArrayList.DEFAULT_CAPACITY);
      removed = (Integer)list.remove(0);
      pass &= test(removed == 1, "expected 1, found " + removed);
      expected = new Integer[] {2};
      pass &= testList(list, expected, 1, BasicArrayList.DEFAULT_CAPACITY);

      // Now remove the remaining one...
      removed = (Integer)list.remove(0);
      pass &= test(removed == 2, "expected 2, found " + removed);
      expected = new Integer[] {};
      pass &= testList(list, expected, 0, BasicArrayList.DEFAULT_CAPACITY);

      // Add two things and remove the second one...
      expected = new Integer[] {1, 2};
      list.add(1);
      list.add(2);
      pass &= testList(list, expected, 2, BasicArrayList.DEFAULT_CAPACITY);
      removed = (Integer)list.remove(1);
      pass &= test(removed == 2, "expected 2, found " + removed);
      expected = new Integer[] {1};
      pass &= testList(list, expected, 1, BasicArrayList.DEFAULT_CAPACITY);
      
      // Now remove the remaining one...
      removed = (Integer)list.remove(0);
      pass &= test(removed == 1, "expected 1, found " + removed);
      expected = new Integer[] {};
      pass &= testList(list, expected, 0, BasicArrayList.DEFAULT_CAPACITY);

      // Add a bunch (more than 10) and remove them in an interesting order...
      expected = new Integer[] {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};

      for (Object o : expected)
      {
         list.add(o);
      }

      pass &= testList(list, expected, 21, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(20);
      pass &= test(removed == 21, "expected 21, found " + removed);
      expected = new Integer[] {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
      pass &= testList(list, expected, 20, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 1, "expected 1, found " + removed);
      expected = new Integer[] {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
      pass &= testList(list, expected, 19, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(9);
      pass &= test(removed == 11, "expected 11, found " + removed);
      expected = new Integer[] {2,3,4,5,6,7,8,9,10,12,13,14,15,16,17,18,19,20};
      pass &= testList(list, expected, 18, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(1);
      pass &= test(removed == 3, "expected 3, found " + removed);
      expected = new Integer[] {2,4,5,6,7,8,9,10,12,13,14,15,16,17,18,19,20};
      pass &= testList(list, expected, 17, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(15);
      pass &= test(removed == 19, "expected 19, found " + removed);
      expected = new Integer[] {2,4,5,6,7,8,9,10,12,13,14,15,16,17,18,20};
      pass &= testList(list, expected, 16, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 2, "expected 2, found " + removed);
      expected = new Integer[] {4,5,6,7,8,9,10,12,13,14,15,16,17,18,20};
      pass &= testList(list, expected, 15, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 4, "expected 4, found " + removed);
      expected = new Integer[] {5,6,7,8,9,10,12,13,14,15,16,17,18,20};
      pass &= testList(list, expected, 14, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 5, "expected 5, found " + removed);
      expected = new Integer[] {6,7,8,9,10,12,13,14,15,16,17,18,20};
      pass &= testList(list, expected, 13, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 6, "expected 6, found " + removed);
      expected = new Integer[] {7,8,9,10,12,13,14,15,16,17,18,20};
      pass &= testList(list, expected, 12, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 7, "expected 7, found " + removed);
      expected = new Integer[] {8,9,10,12,13,14,15,16,17,18,20};
      pass &= testList(list, expected, 11, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 8, "expected 8, found " + removed);
      expected = new Integer[] {9,10,12,13,14,15,16,17,18,20};
      pass &= testList(list, expected, 10, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 9, "expected 9, found " + removed);
      expected = new Integer[] {10,12,13,14,15,16,17,18,20};
      pass &= testList(list, expected, 9, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 10, "expected 10, found " + removed);
      expected = new Integer[] {12,13,14,15,16,17,18,20};
      pass &= testList(list, expected, 8, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 12, "expected 12, found " + removed);
      expected = new Integer[] {13,14,15,16,17,18,20};
      pass &= testList(list, expected, 7, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 13, "expected 13, found " + removed);
      expected = new Integer[] {14,15,16,17,18,20};
      pass &= testList(list, expected, 6, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 14, "expected 14, found " + removed);
      expected = new Integer[] {15,16,17,18,20};
      pass &= testList(list, expected, 5, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 15, "expected 15, found " + removed);
      expected = new Integer[] {16,17,18,20};
      pass &= testList(list, expected, 4, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 16, "expected 16, found " + removed);
      expected = new Integer[] {17,18,20};
      pass &= testList(list, expected, 3, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 17, "expected 17, found " + removed);
      expected = new Integer[] {18,20};
      pass &= testList(list, expected, 2, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 18, "expected 18, found " + removed);
      expected = new Integer[] {20};
      pass &= testList(list, expected, 1, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      removed = (Integer)list.remove(0);
      pass &= test(removed == 20, "expected 20, found " + removed);
      expected = new Integer[] {};
      pass &= testList(list, expected, 0, BasicArrayList.DEFAULT_CAPACITY * 2 * 2);

      return pass;
   }
   
   private static boolean testSet()
   {
      System.out.println("Testing set(int index, Object o)...");
      boolean pass = true;
      boolean caught;
      String s;
      Object[] expected;

      BasicArrayList list = new BasicArrayList();

      try
      {
         caught = false;
         list.set(-99, "test");
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "did not throw expected IndexOutOfBoundsException"); 

      try
      {
         caught = false;
         list.set(0, "test");
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "did not throw expected IndexOutOfBoundsException"); 

      try
      {
         caught = false;
         list.set(99, "test");
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }

      pass &= test(caught, "did not throw expected IndexOutOfBoundsException"); 

      list.add("Hello");

      s = (String)list.set(0, "there");
      pass &= test(s.equals("Hello"), "expected " + "\"Hello\", found " + s);
      
      s = (String)list.set(0, "there");
      pass &= test(s.equals("there"), "expected " + "\"there\", found " + s);

      list.add("how");
      list.add("are");
      list.add("you?");

      s = (String)list.set(1, "XXX");
      pass &= test(s.equals("how"), "expected " + "\"how\", found " + s);

      s = (String)list.set(3, "YYY");
      pass &= test(s.equals("you?"), "expected " + "\"you?\", found " + s);

      pass &= testList(list,
                       new String[] {"there","XXX","are", "YYY"},
                       4,
                       BasicArrayList.DEFAULT_CAPACITY); 

      return pass;
   }

   private static boolean testTrimToSize()
   {
      System.out.println("Testing trimToSize()...");
      boolean pass = true;
      Integer[] expected;

      BasicArrayList list = new BasicArrayList();

      list.trimToSize();
      pass &= testList(list, new Object[]{}, 0, BasicArrayList.DEFAULT_CAPACITY);

      expected = new Integer[] {10, 11, 12, 13, 14};

      for (int i = 0; i < expected.length; i++)
      {
         list.add(expected[i]);
      }

      pass &= testList(list, expected, expected.length, BasicArrayList.DEFAULT_CAPACITY);

      list.trimToSize();
      pass &= testList(list, expected, expected.length, BasicArrayList.DEFAULT_CAPACITY);

      expected = new Integer[] {10,11,12,13,14,15,16,17,18,19,20};

      for (int i = 5; i < expected.length; i++)
      {
         list.add(expected[i]);
      }

      pass &= testList(list, expected, expected.length, BasicArrayList.DEFAULT_CAPACITY * 2);

      list.trimToSize();
      pass &= testList(list, expected, expected.length, expected.length);

      list.add(99);
      expected = new Integer[] {10,11,12,13,14,15,16,17,18,19,20,99};
      pass &= testList(list, expected, expected.length, (expected.length - 1) * 2);

      list.trimToSize();
      pass &= testList(list, expected, expected.length, expected.length);

      // Shrink size to < 10, then trim
      for (int i = 0; i < 5; i++)
      {
         list.remove(0);
      }

      expected = new Integer[] {15, 16, 17, 18, 19, 20, 99};
      pass &= testList(list, expected, expected.length, expected.length + 5);

      list.trimToSize();
      pass &= testList(list, expected, expected.length, BasicArrayList.DEFAULT_CAPACITY);

      return pass;
   }
   
   private static boolean testNullSupport()
   {
      System.out.println("Testing null support...");
      boolean pass = true;
   
      BasicArrayList list = new BasicArrayList();
      Integer[] expected
         = new Integer[] {null, 1, null, 3, null, 5, null, 7, null, 9};

      for (int i = 0; i < expected.length; i++)
      {
         list.add(expected[i]);
         pass &= test(list.size() == i+1, "Expected size " + (i+1)
                                        + ",found " + list.size());
         pass &= test(list.capacity() == BasicArrayList.DEFAULT_CAPACITY,
                      "Expected capacity of " + BasicArrayList.DEFAULT_CAPACITY
                    + ", found " + list.capacity());
         
         for (int j = 0; j <= i; j++)
         {
            if (j % 2 == 1)
            {
               pass &= test((Integer)list.get(j) == j,
                            "Expected " + j + ", found " + list.get(j));
            }
            else
            {
               pass &= test(list.get(j) == null,
                            "Expected null, found " + list.get(j));
            }
         }
      }

      list.add(null);
      pass &= test(list.size() == 11, "Expected size 11, found " + list.size());
      pass &= test(list.capacity() == 2 * BasicArrayList.DEFAULT_CAPACITY,
                   "Expected capacity of " + 2 * BasicArrayList.DEFAULT_CAPACITY
                 + ", found " + list.capacity());
      
      list.add(11);
      pass &= test(list.size() == 12, "Expected size 12, found " + list.size());
      pass &= test(list.capacity() == 2 * BasicArrayList.DEFAULT_CAPACITY,
                   "Expected capacity of " + 2 * BasicArrayList.DEFAULT_CAPACITY
                 + ", found " + list.capacity());
         
      for (int j = 0; j < 12; j++)
      {
         if (j % 2 == 1)
         {
            pass &= test((Integer)list.get(j) == j,
                         "Expected " + j + ", found " + list.get(j));
         }
         else
         {
            pass &= test(list.get(j) == null,
                         "Expected null, found " + list.get(j));
         }
      }

      return pass;
   }

   private static boolean testList(BasicArrayList list, Object[] expected, int size, int capacity)
   {
      boolean pass = true;
      boolean caught = false;
      Object actual[] = list.unusualMethodForTestDriverOnly();
   
      // Try get() with out-of-bounds indexes...
      try
      {
         caught = false;
         list.get(size);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "get(size) did not throw IndexOutOfBoundsException");

      // Try get() with out-of-bounds indexes...
      try
      {
         caught = false;
         list.get(-99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "get(-99) did not throw IndexOutOfBoundsException");
      
      // Try get() with out-of-bounds indexes...
      try
      {
         caught = false;
         list.get(size + 99);
      }
      catch(IndexOutOfBoundsException e)
      {
         caught = true;
      }
      
      pass &= test(caught, "get(size + 99) did not throw IndexOutOfBoundsException");

      // verify size
      pass &= test(list.size() == size, "size() - expected " + size + ", found " + list.size());

      // verify capacity
      pass &= test(list.capacity() == capacity,
              "capacity() - expected " + capacity + ", found " + list.capacity());

      pass &= test(actual.length == capacity,  
              "length of actual array - expected " + capacity + ", found " + actual.length);

      // verify get(int index)
      for (int i = 0; i < size; i++)
      {
         pass &= test(list.get(i).equals(expected[i]),
                 "get(" + i + ") - expected " + expected[i] + ", found " + list.get(i));
      }
      
      // verify contains(Object o)
      for (Object o : expected)
      {
         pass &= test(list.contains(o), "contains(Object o) - expected true, found false");
      }

      // verify indexOf(Object o)
      for (int i = 0; i < expected.length; i++)
      {
         pass &= test(list.indexOf(expected[i]) == i, "indexOf(Object o) - expected " + i
                      + "found " + list.indexOf(expected[i]));
      }
   
      caught = false;
      
      try
      {
         list.indexOf(new Object());
      }
      catch(NoSuchElementException e)
      {
         caught = true;
      }

      pass &= test(caught, "indexOf() did not throw expected NoSuchElementException"); 
      pass &= test(!list.contains(new java.awt.Point(-6, 13)),
              "contains(Object o) - expected false, found true");

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
         msg = "\nCongratulations, you have passed all the tests!\n\n"
            + "Your grade will be based on when you turn in your functionally\n"
            + "correct solution and any deductions for the quality of your\n"
            + "implementation.  Quality is based on, but not limited to,\n"
            + "coding style, documentation requirements, compiler warnings,\n"
            + "and the efficiency and elegance of your code.\n";
      }
      else
      {
         msg = "\nNot done yet - you failed one or more tests!\n";
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

   /* Checks for use of disallowed Library classes */
   private static boolean checkInstanceVars(Field[] fields)
   {
      for (Field f : fields)
      {
         if (f.toString().contains("java.")
          && !f.toString().contains(".lang"))
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
         if (modifier == Modifier.INTERFACE || modifier == m.getModifiers())
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
                - countModifiers(fields, Modifier.PUBLIC)
                - countModifiers(fields, Modifier.PUBLIC + Modifier.STATIC + Modifier.FINAL);
      
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
