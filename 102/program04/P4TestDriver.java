/**
 * Test driver for shapes-interitance assignment.
 *
 * @author Kurt Mammen
 * @version 02/06/2012 - Correct tests for Square regarding setVertex
 * @version 02/05/2012 - Modified tests for simplified overriding of setVertex
 * @version 10/23/2011 - Added Ellipse, setVertex overriding...
 */
import java.awt.Color;
import java.awt.Point;
import java.util.ArrayList;
import java.lang.reflect.*;
import java.util.Arrays;

public class P4TestDriver
{
   private static final String RESULTS_FOR = "Results for Program 4";

   public static void main(String[] args) throws ClassNotFoundException
   {
      boolean pass = true;
      
      printHeader(args);
      
      // Architecture tests...
      pass &= testShapeArch();
      pass &= testConvexPolygonArch();
      pass &= testTriangleArch();
      pass &= testRectangleArch();
      pass &= testSquareArch();
      pass &= testEllipseArch();
      pass &= testCircleArch();
      pass &= testCanvasArch();

      System.out.println();
      
      // Unit Tests...
      pass &= testConvexPolygon();
      pass &= testTriangle();
      pass &= testRectangle();
      pass &= testSquare();
      pass &= testEllipse();
      pass &= testCircle();
      pass &= testCanvas();
      pass &= testCompareTo();
      
      printResults(pass);
      
      // Added for to support robust script checking
      if (!pass)
      {
         System.exit(-1);
      }
   }

   private static boolean testShapeArch() throws ClassNotFoundException
   {
      boolean pass = true;
      int test = 1;
      int cnt;
      Class cl = Shape.class;
      Class[] temp;
      
      System.out.println("Shape architecture tests...");

      pass &= test(Modifier.isAbstract(cl.getModifiers()), test++);
      pass &= test(cl.getConstructors().length == 1, test++);
      
      Type[] types = cl.getGenericInterfaces();
      pass &= test(types.length == 1, test++);
      if (types.length == 1)
      {
         pass &= test(types[0].toString().equals("java.lang.Comparable<Shape>"), test++);
      }

      // Verify concrete methods
      String[] names = {"equals", "getColor", "setColor", "getFilled", "setFilled", "compareTo"};

      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);
      pass &= test(cnt == names.length, test++);
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names), test++);
      
      // Verify abstract methods.
      names = new String[] {"move", "getArea", "getPosition", "setPosition"};

      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC + Modifier.ABSTRACT);
      pass &= test(cnt == names.length, test++);
      pass &= test(verifyNames(cl.getDeclaredMethods(), 
                               Modifier.PUBLIC + Modifier.ABSTRACT,
                               names),
                   test++);
      
      pass &= test(verifyEqualsMethodSignature(cl), test++);
      
      cnt = cl.getFields().length;
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt == 2, test++);
      
      // Count and test number of of PACKAGE fields
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, test++);
      
      return pass;
   }
   
   private static boolean testEllipseArch()
   {
      boolean pass = true;
      int test = 1;
      int cnt;
      Class cl = Ellipse.class;
      Class[] temp;
      
      System.out.println("Ellipse architecture tests...");

      pass &= test(cl.getSuperclass() == Shape.class, test++);
      pass &= test(cl.getConstructors().length == 1, test++);
      
      String[] names = {"move", "getArea", "equals",
                        "getPosition", "setPosition",
                        "getSemiMajorAxis", "setSemiMajorAxis", 
                        "getSemiMinorAxis", "setSemiMinorAxis"}; 
      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);     
      pass &= test(cnt == names.length, test++);
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names), test++);
      pass &= test(verifyEqualsMethodSignature(cl), test++);
      
      cnt = cl.getFields().length;
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt == 3, test++);
      
      // Count and test number of of PACKAGE fields
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, test++);
    
      return pass;
   }

   private static boolean testCircleArch()
   {
      boolean pass = true;
      int test = 1;
      int cnt;
      Class cl = Circle.class;
      Class[] temp;
      
      System.out.println("Circle architecture tests...");

      pass &= test(cl.getSuperclass() == Ellipse.class, test++);
      pass &= test(cl.getConstructors().length == 1, test++);
      
      String[] names = {"getRadius", "setRadius", 
                        "setSemiMajorAxis", "setSemiMinorAxis"}; 
      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);     
      pass &= test(cnt == names.length, test++);
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names), test++);
      
      cnt = cl.getFields().length;
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt == 0, test++);
      
      // Count and test number of of PACKAGE fields
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, test++);
    
      return pass;
   }

   private static boolean testRectangleArch()
   {
      boolean pass = true;
      int test = 1;
      int cnt;
      Class cl = Rectangle.class;
      Class[] temp;
      
      System.out.println("Rectangle architecture tests...");

      pass &= test(cl.getSuperclass() == ConvexPolygon.class, test++);
      pass &= test(cl.getConstructors().length == 1, test++);     

      String[] names = {"getWidth", "setWidth", "getHeight", "setHeight", "setVertex"};

      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);     
      pass &= test(cnt == names.length, test++);
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names), test++);
      
      cnt = cl.getFields().length;
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt == 0, test++);
      
      // Count and test number of of PACKAGE fields
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, test++);

      return pass;
   }

   private static boolean testSquareArch()
   {
      boolean pass = true;
      int test = 1;
      int cnt;
      Class cl = Square.class;
      Class[] temp;
      
      System.out.println("Square architecture tests...");

      pass &= test(cl.getSuperclass() == Rectangle.class, test++);
      pass &= test(cl.getConstructors().length == 1, test++);
      
      String[] names = {"setSize", "getSize", "setWidth", "setHeight"};
      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);     
      pass &= test(cnt == names.length, test++);
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names), test++);
      
      cnt = cl.getFields().length;
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt == 0, test++);
      
      // Count and test number of of PACKAGE fields
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, test++);
         
      return pass;
   }
   
   private static boolean testTriangleArch()
   {
      boolean pass = true;
      int test = 1;
      int cnt;
      Class cl = Triangle.class;
      Class[] temp;
      
      System.out.println("Triangle architecture tests...");

      pass &= test(cl.getSuperclass() == ConvexPolygon.class, test++);
      pass &= test(cl.getConstructors().length == 1, test++);
      
      String[] names1 = {"getVertexA", "getVertexB", "getVertexC",
                         "setVertexA", "setVertexB", "setVertexC"};
                    
      String[] names2 = {"getVertexA", "getVertexB", "getVertexC",
                         "setVertexA", "setVertexB", "setVertexC",
                         "getArea"};
                    
      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);   

      // Allows area to be overridden or not, as student prefered
      if (cnt == names2.length)
      {
         pass &= test(cnt == names2.length, test++);
         pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names2), test++);
      }
      else
      {
         pass &= test(cnt == names1.length, test++);
         pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names1), test++);
      }

      cnt = cl.getFields().length;
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt == 0, test++);
      
      // Count and test number of of PACKAGE fields
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, test++);

      return pass;
   }

   private static boolean testConvexPolygonArch()
   {
      boolean pass = true;
      int test = 1;
      int cnt;
      Class cl = ConvexPolygon.class;
      Class[] temp;
      
      System.out.println("ConvexPolygon architecture tests..."); 
      pass &= test(cl.getSuperclass() == Shape.class, test++);
      pass &= test(cl.getConstructors().length == 1, test++);
      
      String[] names = {"getVertex", "setVertex", "equals", "getArea", "move",
                        "getPosition", "setPosition"};

      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);     
      pass &= test(cnt == names.length, test++);
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names), test++);
      pass &= test(verifyEqualsMethodSignature(cl), test++);
      
      cnt = cl.getFields().length;
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt == 1, test++);
      
      // Count and test number of of PACKAGE fields
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, test++);
         
      return pass;
   }
   
   private static boolean testCanvasArch()
   {
      boolean pass = true;
      int test = 1;
      int cnt;
      Class cl = Canvas.class;
      Class[] temp;
      
      System.out.println("Canvas architecture tests...");

      pass &= test(cl.getConstructors().length == 1, test++);
      
      String[] names = {"add", "remove", "get", "size", "getCircles",
                        "getRectangles", "getTriangles", "getConvexPolygons",
                        "getShapesByColor", "getAreaOfAllShapes"};

      cnt = countModifiers(cl.getDeclaredMethods(), Modifier.PUBLIC);     
      pass &= test(cnt == names.length, test++);
      pass &= test(verifyNames(cl.getDeclaredMethods(), Modifier.PUBLIC, names), test++);
      
      cnt = cl.getFields().length;
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PROTECTED);
      pass &= test(cnt == 0, test++);
      
      cnt = countModifiers(cl.getDeclaredFields(), Modifier.PRIVATE);
      pass &= test(cnt == 1, test++);
      
      // Count and test number of of PACKAGE fields
      cnt = countPackage(cl.getDeclaredFields());
      pass &= test(cnt == 0, test++);
         
      return pass;
   }

   private static boolean testEllipse()
   {
      boolean pass = true;
      int test = 1;
      Ellipse ellipse;
      
      System.out.println("Ellipse tests...");
   
      Point pt = new Point(-99, 66);
      ellipse = new Ellipse(5.6789, 2.3456, pt, Color.cyan, true);

      pass &= test(ellipse.getSemiMajorAxis() == 5.6789, test++);
      pass &= test(ellipse.getSemiMinorAxis() == 2.3456, test++);
      pass &= test(ellipse.getPosition().equals(new Point(-99, 66)), test++);
      pass &= test(ellipse.getColor().equals(Color.cyan), test++);
      pass &= test(ellipse.getFilled(), test++);
      pass &= test(approx(ellipse.getArea(), Math.PI*5.6789*2.3456, .000001), test++);
      
      // Deep construction?
      pt.x = pt.y = 0;
      pass &= test(ellipse.getPosition().equals(new Point(-99, 66)), test++);

      // sets and gets...
      ellipse.setSemiMajorAxis(7.8);
      pass &= test(ellipse.getSemiMinorAxis() == 2.3456, test++);
      pass &= test(ellipse.getSemiMajorAxis() == 7.8, test++);
      pass &= test(approx(ellipse.getArea(), Math.PI*7.8*2.3456, .000001), test++);

      ellipse.setSemiMinorAxis(4.321);
      pass &= test(ellipse.getSemiMinorAxis() == 4.321, test++);
      pass &= test(ellipse.getSemiMajorAxis() == 7.8, test++);
      pass &= test(approx(ellipse.getArea(), Math.PI*7.8*4.321, .000001), test++);
      
      /* Sets major axis to less than current minor axis. */
      ellipse.setSemiMajorAxis(3.25);
      pass &= test(ellipse.getSemiMinorAxis() == 3.25, test++);
      pass &= test(ellipse.getSemiMajorAxis() == 4.321, test++);
      pass &= test(approx(ellipse.getArea(), Math.PI*3.25*4.321, .000001), test++);

      /* Sets minor axis to more than current major axis. */
      ellipse.setSemiMinorAxis(8.9);
      pass &= test(ellipse.getSemiMinorAxis() == 4.321, test++);
      pass &= test(ellipse.getSemiMajorAxis() == 8.9, test++);
      pass &= test(approx(ellipse.getArea(), Math.PI*4.321*8.9, .000001), test++);
      
      // Shape stuff...
      ellipse.setColor(Color.red);
      pass &= test(ellipse.getColor().equals(Color.red), test++);

      ellipse.setFilled(false);
      pass &= test(!ellipse.getFilled(), test++);
      
      // Deep get?
      pt = ellipse.getPosition();
      pt.x = pt.y = 0;
      pass &= test(ellipse.getPosition().equals(new Point(-99, 66)), test++);

      // Deep set?
      pt.x = pt.y = -1;
      ellipse.setPosition(pt);
      pt.x = pt.y = -2;
      pass &= test(ellipse.getPosition().equals(new Point(-1, -1)), test++);

      // Test move?
      pt.x = -1;
      pt.y = -2;
      ellipse.move(pt);
      pass &= test(ellipse.getPosition().equals(new Point(-2, -3)), test++);
     
      // equals...
      ellipse.setPosition(new Point(-104, 59));
      ellipse.setSemiMajorAxis(8.9);
      ellipse.setSemiMinorAxis(3.25);
      ellipse.setColor(Color.cyan);
      ellipse.setFilled(true);
      
      Ellipse e2 = new Ellipse(8.9, 3.25, new Point(-104, 59), Color.cyan, true);
      pass &= test(ellipse.equals(e2), test++);
      
      e2 = new Ellipse(8.9, 3.25, new Point(-104, 59), Color.cyan, false);
      pass &= test(!ellipse.equals(e2), test++);

      e2 = new Ellipse(8.9, 3.25, new Point(-104, 59), Color.red, true);
      pass &= test(!ellipse.equals(e2), test++);
 
      e2 = new Ellipse(8.9, 3.25, new Point(104, 59), Color.cyan, true);
      pass &= test(!ellipse.equals(e2), test++);

      e2 = new Ellipse(8.9, 3.25, new Point(-104, -59), Color.cyan, true);
      pass &= test(!ellipse.equals(e2), test++);

      e2 = new Ellipse(8.91, 3.25, new Point(-104, 59), Color.cyan, true);
      pass &= test(!ellipse.equals(e2), test++);
                   
      e2 = new Ellipse(8.9, 3.251, new Point(-104, 59), Color.cyan, true);
      pass &= test(!ellipse.equals(e2), test++);
                   
      pass &= test(!e2.equals(null), test++);
      pass &= test(!e2.equals(new String("Whatever")), test++);

      return pass;  
   }
   
   private static boolean testCircle()
   {
      boolean pass = true;
      int test = 1;
      Circle circle;
      
      System.out.println("Circle tests...");
   
      Point pt = new Point(-99, 66);
      circle = new Circle(5.6789, pt, Color.cyan, true);

      pass &= test(circle.getRadius() == 5.6789, test++);
      pass &= test(circle.getSemiMajorAxis() == 5.6789, test++);
      pass &= test(circle.getSemiMinorAxis() == 5.6789, test++);
      pass &= test(circle.getPosition().equals(new Point(-99, 66)), test++);
      pass &= test(circle.getColor().equals(Color.cyan), test++);
      pass &= test(circle.getFilled(), test++);
      pass &= test(approx(circle.getArea(), Math.PI*5.6789*5.6789, .000001), test++);
      
      // sets and gets...
      circle.setRadius(4.321);
      pass &= test(circle.getRadius() == 4.321, test++);
      pass &= test(circle.getSemiMajorAxis() == 4.321, test++);
      pass &= test(circle.getSemiMinorAxis() == 4.321, test++);
      pass &= test(approx(circle.getArea(), Math.PI*4.321*4.321, 0.000001), test++);

      circle.setSemiMajorAxis(7.8);
      pass &= test(circle.getRadius() == 7.8, test++);
      pass &= test(circle.getSemiMajorAxis() == 7.8, test++);
      pass &= test(circle.getSemiMinorAxis() == 7.8, test++);
      pass &= test(approx(circle.getArea(), Math.PI*7.8*7.8, 0.000001), test++);
      
      circle.setSemiMinorAxis(3.2);
      pass &= test(circle.getRadius() == 3.2, test++);
      pass &= test(circle.getSemiMajorAxis() == 3.2, test++);
      pass &= test(circle.getSemiMinorAxis() == 3.2, test++);
      pass &= test(approx(circle.getArea(), Math.PI*3.2*3.2, 0.000001), test++);

      return pass;  
   }
   
   private static boolean testRectangle()
   {
      boolean pass = true;
      int test = 1;
      Rectangle rect;
      Point pt;
      
      System.out.println("Rectangle tests...");
   
      pt = new Point(-99, 66);
      rect = new Rectangle(4, 5, pt, Color.cyan, true);
      
      pass &= test(rect.getWidth() == 4, test++);
      pass &= test(rect.getHeight() == 5, test++);
      pass &= test(rect.getArea() == 4 * 5, test++);
      pass &= test(rect.getPosition().equals(new Point(-99, 66)), test++);
      pass &= test(rect.getColor().equals(Color.cyan), test++);
      pass &= test(rect.getFilled(), test++);

      // Deep construction
      pt.x = pt.y = 0;
      pass &= test(rect.getWidth() == 4, test++);
      pass &= test(rect.getHeight() == 5, test++);
      pass &= test(rect.getArea() == 4 * 5, test++);
      pass &= test(rect.getPosition().equals(new Point(-99, 66)), test++); 

      rect.setWidth(7); 
      pass &= test(rect.getWidth() == 7, test++);
      pass &= test(rect.getArea() == 7 * 5, test++);
      pass &= test(rect.getPosition().equals(new Point(-99, 66)), test++);

      rect.setHeight(89);
      pass &= test(rect.getHeight() == 89, test++);
      pass &= test(rect.getArea() == 7 * 89, test++);
      pass &= test(rect.getPosition().equals(new Point(-99, 66)), test++);

      // Test setVertex override...
      rect = new Rectangle(4, 4, new Point(-2, -4), Color.blue, false);

      boolean caught = false;

      try
      {
         rect.setVertex(0, new Point(-3, -4));
      }
      catch(UnsupportedOperationException e)
      {
         caught = true;
      }

      pass &= test(caught, test++);

      return pass;
   }
   
   private static boolean testSquare()
   {
      boolean pass = true;
      int test = 1;
      Square sq;
      
      System.out.println("Square tests...");
   
      Point pt = new Point(-99, 66);
      sq = new Square(4, pt, Color.cyan, true);
      
      pass &= test(sq.getSize() == 4, test++);
      pass &= test(sq.getWidth() == 4, test++);
      pass &= test(sq.getHeight() == 4, test++);
      pass &= test(sq.getPosition().equals(new Point(-99, 66)), test++);
      pass &= test(sq.getColor().equals(Color.cyan), test++);
      pass &= test(sq.getFilled(), test++);
      
      // Deep construction?
      pt.x = pt.y = -5;
      pass &= test(sq.getPosition().equals(new Point(-99, 66)), test++);
      
      // Sets and gets...
      sq.setSize(7);
      pass &= test(sq.getSize() == 7, test++);
      pass &= test(sq.getWidth() == 7, test++);
      pass &= test(sq.getHeight() == 7, test++);
      pass &= test(sq.getPosition().equals(new Point(-99, 66)), test++);

      sq.setWidth(8);
      pass &= test(sq.getSize() == 8, test++);
      pass &= test(sq.getWidth() == 8, test++);
      pass &= test(sq.getHeight() == 8, test++);
      pass &= test(sq.getPosition().equals(new Point(-99, 66)), test++);

      sq.setHeight(9);
      pass &= test(sq.getSize() == 9, test++);
      pass &= test(sq.getWidth() == 9, test++);
      pass &= test(sq.getHeight() == 9, test++);
      pass &= test(sq.getPosition().equals(new Point(-99, 66)), test++);

      return pass;
   }
   
   private static boolean testTriangle()
   {
      boolean pass = true;
      int test = 1;
      Triangle tri;
      Point a = new Point(0, 0);
      Point b = new Point(3, 0);
      Point c = new Point(0, 4);
      
      System.out.println("Triangle tests...");
 
      tri = new Triangle(a, b, c, Color.cyan, true);

      pass &= test(tri.getVertexA().equals(new Point(0, 0)), test++);
      pass &= test(tri.getVertexB().equals(new Point(3, 0)), test++);
      pass &= test(tri.getVertexC().equals(new Point(0, 4)), test++);
      pass &= test(tri.getPosition().equals(new Point(0, 0)), test++);
      pass &= test(approx(tri.getArea(), 6.0, 0.000001), test++);
      pass &= test(tri.getColor().equals(Color.cyan), test++);
      pass &= test(tri.getFilled(), test++);

      // Clockwise points
      tri = new Triangle(c, b, a, Color.cyan, true);
      pass &= test(approx(tri.getArea(), 6.0, 0.000001), test++);

      // deep construction?
      a.x = a.y = -1;
      b.x = b.y = -2;
      c.x = c.y = -3;

      pass &= test(tri.getVertexA().equals(new Point(0, 4)), test++);
      pass &= test(tri.getVertexB().equals(new Point(3, 0)), test++);
      pass &= test(tri.getVertexC().equals(new Point(0, 0)), test++);
  
      // deep gets...
      Point pt = tri.getVertexA();
      pt.x = pt.y = -1;
      pass &= test(tri.getVertexA().equals(new Point(0, 4)), test++);

      pt = tri.getVertexB();
      pt.x = pt.y = -1;
      pass &= test(tri.getVertexB().equals(new Point(3, 0)), test++);

      pt = tri.getVertexC();
      pt.x = pt.y = -1;
      pass &= test(tri.getVertexC().equals(new Point(0, 0)), test++);
      
      // deep sets...
      pt.x = pt.y = 0;
      tri.setVertexA(pt);
      pt.x = pt.y = -1;
      pass &= test(tri.getVertexA().equals(new Point(0, 0)), test++);      

      pt.x = 0;
      pt.y = 1;
      tri.setVertexB(pt);
      pt.x = pt.y = -1;
      pass &= test(tri.getVertexB().equals(new Point(0, 1)), test++);

      pt.x = 1;
      pt.y = 1;
      tri.setVertexC(pt);
      pt.x = pt.y = -1;
      pass &= test(tri.getVertexC().equals(new Point(1, 1)), test++);
 
      pass &= test(approx(tri.getArea(), 0.5, 0.000001), test++);

      return pass;  
   }
   
   private static boolean testConvexPolygon()
   {
      boolean pass = true;
      int test = 1;
      ConvexPolygon poly;
      Point a = new Point(7, 7);
      Point b = new Point(0, 9);
      Point c = new Point(-3, -5);
      Point d = new Point(2, -6);
      Point e = new Point(1257, 0);
      Point[] vertices = new Point[5];
      Point ta, tb, tc, td, te;
      vertices[0] = ta = new Point(a);
      vertices[1] = tb = new Point(b);
      vertices[2] = tc = new Point(c);
      vertices[3] = td = new Point(d);
      vertices[4] = te = new Point(e);
         
      System.out.println("ConvexPolygon tests...");

      poly = new ConvexPolygon(vertices, Color.cyan, true);

      pass &= test(poly.getVertex(0).equals(a), test++);
      pass &= test(poly.getVertex(1).equals(b), test++);
      pass &= test(poly.getVertex(2).equals(c), test++);
      pass &= test(poly.getVertex(3).equals(d), test++);
      pass &= test(poly.getVertex(4).equals(e), test++);
      pass &= test(poly.getColor().equals(Color.cyan), test++);
      pass &= test(poly.getFilled(), test++);
      pass &= test(approx(poly.getArea(), 8229.5, 0.000001), test++);
      pass &= test(poly.getPosition().equals(a), test++);

      // Deep constructor?
      ta.x = tb.x = tc.x = td.x = te.x = 0;
      ta.y = tb.y = tc.y = td.y = te.y = 0;
      pass &= test(poly.getVertex(0).equals(a), test++);
      pass &= test(poly.getVertex(1).equals(b), test++);
      pass &= test(poly.getVertex(2).equals(c), test++);
      pass &= test(poly.getVertex(3).equals(d), test++);
      pass &= test(poly.getVertex(4).equals(e), test++);
      
      // Deep getVertex?
      Point pt = poly.getVertex(2);
      pt.x = pt.y = -1;
      pass &= test(poly.getVertex(2).equals(c), test++);

      // Deep setVertex?
      poly.setVertex(2, pt);
      pt.x = pt.y = -2;
      pass &= test(poly.getVertex(2).equals(new Point(-1, -1)), test++);
      poly.setVertex(2, c); // return to original state

      // Test setPosition
      pt = new Point(-1, 2);
      poly.setPosition(pt);
      pass &= test(poly.getVertex(0).equals(new Point(-1, 2)), test++);
      pass &= test(poly.getVertex(1).equals(new Point(-8, 4)), test++);
      pass &= test(poly.getVertex(2).equals(new Point(-11, -10)), test++);
      pass &= test(poly.getVertex(3).equals(new Point(-6, -11)), test++);
      pass &= test(poly.getVertex(4).equals(new Point(1249, -5)), test++);   

      // Test getPosition
      pass &= test(poly.getPosition().equals(new Point(-1, 2)), test++);

      // Deep?
      pt.x = pt.y = -99;
      pass &= test(poly.getPosition().equals(new Point(-1, 2)), test++);

      pt = poly.getPosition();
      pt.x = pt.y = -88;
      pass &= test(poly.getPosition().equals(new Point(-1, 2)), test++);

      // Test move
      pt = new Point(4, 31);
      poly.move(pt);
      pass &= test(poly.getVertex(0).equals(new Point(3, 33)), test++);
      pass &= test(poly.getVertex(1).equals(new Point(-4, 35)), test++);
      pass &= test(poly.getVertex(2).equals(new Point(-7, 21)), test++);
      pass &= test(poly.getVertex(3).equals(new Point(-2, 20)), test++);
      pass &= test(poly.getVertex(4).equals(new Point(1253, 26)), test++);   

      // Shape stuff...
      poly.setColor(Color.red);
      pass &= test(poly.getColor().equals(Color.red), test++);

      poly.setFilled(false);
      pass &= test(!poly.getFilled(), test++);

      // Test equals
      poly = new ConvexPolygon(vertices, Color.cyan, true);

      ConvexPolygon poly2 = new ConvexPolygon(vertices, Color.cyan, true);
      System.out.println("vertices[0]: " + vertices[0]);
      System.out.println("poly.getVertex(0) " + poly.getVertex(0));
      pass &= test(poly.equals(poly2), test++);
      
      poly2.setVertex(4, new Point(2, 77));
      pass &= test(!poly.equals(poly2), test++);  

      poly2 = new ConvexPolygon(vertices, Color.blue, true); 
      pass &= test(!poly.equals(poly2), test++);

      poly2 = new ConvexPolygon(vertices, Color.cyan, false); 
      pass &= test(!poly.equals(poly2), test++); 

      pass &= test(!poly.equals(null), test++);
      pass &= test(!poly.equals("whatever"), test++);

      return pass;   
   }
   
   private static boolean testCanvas()
   {
      boolean pass = true;
      int test = 1;
      double area;
      Canvas canvas = new Canvas();
      Circle[] circles = new Circle[3];
      Rectangle[] rects = new Rectangle[3];
      Square[] squares = new Square[3];
      Triangle[] tris = new Triangle[3];
      ConvexPolygon[] polys = new ConvexPolygon[3];
      ArrayList<Circle> circleList;
      ArrayList<Rectangle> rectList;
      ArrayList<Triangle> triList;
      ArrayList<ConvexPolygon> polyList;
      ArrayList<Shape> shapeList;
      ArrayList<Square> squareList;
      
      circles[0] = new Circle(1.1, new Point(1, 2), Color.cyan, false);
      circles[1] = new Circle(2.2, new Point(2, 3), Color.red, false);   
      circles[2] = new Circle(3.3, new Point(3, 4), Color.black, false);
      
      rects[0] = new Rectangle(1, 1, new Point(1, 2), Color.cyan, false);
      rects[1] = new Rectangle(2, 2, new Point(2, 3), Color.red, false);
      rects[2] = new Rectangle(3, 3, new Point(3, 4), Color.black, false);
      
      squares[0] = new Square(1, new Point(1, 2), Color.cyan, false);
      squares[1] = new Square(2, new Point(2, 3), Color.red, false);
      squares[2] = new Square(3, new Point(3, 4), Color.black, false);

      Point a = new Point(1, 1);
      Point b = new Point(0, 2);
      Point c = new Point(0, 0);
      
      Point aa = new Point( 2, 2);
      Point bb = new Point(1, 3);
      Point cc = new Point(1, 1);
      
      Point aaa = new Point(3, 3);
      Point bbb = new Point(2, 4);
      Point ccc = new Point(2, 2);

      tris[0] = new Triangle(a, b, c, Color.cyan, false);
      tris[1] = new Triangle(aa, bb, cc, Color.red, false);
      tris[2] = new Triangle(aaa, bbb, ccc, Color.black, false);
      
      Point[] aVertices = new Point[5];
      Point[] bVertices = new Point[5];
      Point[] cVertices = new Point[5];
      
      aVertices[0] = new Point(4, 0);
      aVertices[1] = new Point(2, 2);
      aVertices[2] = new Point(-2, -2);
      aVertices[3] = new Point(-4, 0);
      aVertices[4] = new Point(0, -2);

      bVertices[0] = new Point(4, 1);
      bVertices[1] = new Point(2, 3);
      bVertices[2] = new Point(-2, -1);
      bVertices[3] = new Point(-4, 1);
      bVertices[4] = new Point(0, -1);

      cVertices[0] = new Point(4, -1);
      cVertices[1] = new Point(2, 1);
      cVertices[2] = new Point(-2, -3);
      cVertices[3] = new Point(-4, -1);
      cVertices[4] = new Point(0, -3);
     
      polys[0] = new ConvexPolygon(aVertices, Color.cyan, false);
      polys[1] = new ConvexPolygon(bVertices, Color.red, false);
      polys[2] = new ConvexPolygon(cVertices, Color.black, false);
      
      System.out.println("Canvas tests...");
      
      // Test an empty WorkSpace...
      pass &= test(canvas.size() == 0, test++);
      pass &= test(canvas.getCircles().size() == 0, test++);
      pass &= test(canvas.getRectangles().size() == 0, test++);
      pass &= test(canvas.getTriangles().size() == 0, test++);
      pass &= test(canvas.getConvexPolygons().size() == 0, test++);
      pass &= test(canvas.getShapesByColor(Color.cyan).size() == 0, test++);
      pass &= test(canvas.getAreaOfAllShapes() == 0, test++);
      
      // Add a shape and test a WorkSpace with one shape in it...
      canvas.add(circles[0]);

      pass &= test(canvas.size() == 1, test++);
      pass &= test(canvas.getCircles().size() == 1, test++);
      pass &= test(canvas.getRectangles().size() == 0, test++);
      pass &= test(canvas.getTriangles().size() == 0, test++);
      pass &= test(canvas.getConvexPolygons().size() == 0, test++);
      pass &= test(canvas.getShapesByColor(Color.black).size() == 0, test++);
      pass &= test(canvas.getShapesByColor(Color.cyan).size() == 1, test++);
      pass &= test(canvas.getCircles().get(0).equals(circles[0]), test++);
      pass &= test(canvas.getShapesByColor(Color.cyan).get(0).equals(circles[0]), test++);
      pass &= test(canvas.get(0).equals(circles[0]), test++);
      pass &= test(approx(canvas.getAreaOfAllShapes(), circles[0].getArea(), 0.000001), test++);
      
      // Remove a shape and test an empty WorkSpace...
      pass &= test(canvas.remove(0).equals(circles[0]), test++);
      pass &= test(canvas.size() == 0, test++);
      pass &= test(canvas.getCircles().size() == 0, test++);
      pass &= test(canvas.getRectangles().size() == 0, test++);
      pass &= test(canvas.getTriangles().size() == 0, test++);
      pass &= test(canvas.getConvexPolygons().size() == 0, test++);
      pass &= test(canvas.getShapesByColor(Color.cyan).size() == 0, test++);
      pass &= test(canvas.getAreaOfAllShapes() == 0, test++);
      
      
      // Add one of each shape and test...
      canvas.add(circles[0]);
      canvas.add(rects[0]);
      canvas.add(tris[0]);
      canvas.add(polys[0]);
      canvas.add(squares[0]);
      
      pass &= test(canvas.size() == 5, test++);
      pass &= test(canvas.getCircles().size() == 1, test++);
      pass &= test(canvas.getRectangles().size() == 1, test++);
      pass &= test(canvas.getTriangles().size() == 1, test++);
      pass &= test(canvas.getConvexPolygons().size() == 1, test++);
      pass &= test(canvas.getShapesByColor(Color.black).size() == 0, test++);
      pass &= test(canvas.getShapesByColor(Color.cyan).size() == 5, test++);
      pass &= test(canvas.getCircles().get(0).equals(circles[0]), test++);
      pass &= test(canvas.getRectangles().get(0).equals(rects[0]), test++);
      pass &= test(canvas.getTriangles().get(0).equals(tris[0]), test++);
      pass &= test(canvas.getConvexPolygons().get(0).equals(polys[0]), test++);
      
      area = circles[0].getArea();
      area += rects[0].getArea();
      area += tris[0].getArea();
      area += polys[0].getArea();
      area += squares[0].getArea();
      
      pass &= test(approx(canvas.getAreaOfAllShapes(), area, 0.000001), test++);

      // Remove a shape and test again...
      canvas.remove(2);
      
      pass &= test(canvas.size() == 4, test++);
      pass &= test(canvas.getCircles().size() == 1, test++);
      pass &= test(canvas.getRectangles().size() == 1, test++);
      pass &= test(canvas.getTriangles().size() == 0, test++);
      pass &= test(canvas.getConvexPolygons().size() == 1, test++);
      pass &= test(canvas.getShapesByColor(Color.black).size() == 0, test++);
      pass &= test(canvas.getShapesByColor(Color.cyan).size() == 4, test++);
      pass &= test(canvas.getCircles().get(0).equals(circles[0]), test++);
      pass &= test(canvas.getRectangles().get(0).equals(rects[0]), test++);
      pass &= test(canvas.getConvexPolygons().get(0).equals(polys[0]), test++);
      
      area -= tris[0].getArea();

      pass &= test(approx(canvas.getAreaOfAllShapes(), area, 0.000001), test++);

      // Add more shapes and test again...
      canvas.add(tris[0]);
      area += tris[0].getArea();
            
      for(int i = 1; i < 3; i++)
      {
         canvas.add(circles[i]);
         canvas.add(rects[i]);
         canvas.add(tris[i]);
         canvas.add(polys[i]);
         
         area += circles[i].getArea();
         area += rects[i].getArea();
         area += tris[i].getArea();
         area += polys[i].getArea();
      }
      
      canvas.add(squares[1]);
      canvas.add(squares[2]);
      area += squares[1].getArea();
      area += squares[2].getArea();
    
      pass &= test(canvas.size() == 15, test++);
      pass &= test(canvas.getCircles().size() == 3, test++);
      pass &= test(canvas.getRectangles().size() == 3, test++);
      pass &= test(canvas.getTriangles().size() == 3, test++);
      pass &= test(canvas.getConvexPolygons().size() == 3, test++);
      pass &= test(canvas.getShapesByColor(Color.black).size() == 5, test++);
      pass &= test(canvas.getShapesByColor(Color.cyan).size() == 5, test++);
      pass &= test(canvas.getShapesByColor(Color.white).size() == 0, test++);
      pass &= test(canvas.get(9).equals(circles[2]), test++);
      pass &= test(canvas.get(10).equals(rects[2]), test++);
      pass &= test(canvas.get(11).equals(tris[2]), test++);
      pass &= test(canvas.get(12).equals(polys[2]), test++);
      pass &= test(approx(canvas.getAreaOfAllShapes(), area, 0.000001), test++);

      // Remove a couple of shapes and test again...
      pass &= test(canvas.remove(0).equals(circles[0]), test++);
      pass &= test(canvas.remove(11).equals(polys[2]), test++);
      pass &= test(canvas.remove(8).equals(circles[2]), test++);
      
      pass &= test(canvas.size() == 12, test++);
      pass &= test(canvas.getCircles().size() == 1, test++);
      pass &= test(canvas.getRectangles().size() == 3, test++);
      pass &= test(canvas.getTriangles().size() == 3, test++);
      pass &= test(canvas.getConvexPolygons().size() == 2, test++);
      pass &= test(canvas.getShapesByColor(Color.black).size() == 3, test++);
      pass &= test(canvas.getShapesByColor(Color.cyan).size() == 4, test++);
      pass &= test(canvas.getShapesByColor(Color.white).size() == 0, test++);
      
      area -= circles[0].getArea();
      area -= polys[2].getArea();
      area -= circles[2].getArea();
      
      pass &= test(approx(canvas.getAreaOfAllShapes(), area, 0.000001), test++);
                          
      return pass;     
   }
   
   private static boolean testCompareTo()
   {
      boolean pass = true;
      int test = 1;
      Circle[] circles = new Circle[2];
      Rectangle[] rects = new Rectangle[2];
      Triangle[] tris = new Triangle[2];
      ConvexPolygon[] polys = new ConvexPolygon[2];
      Square[] sqs = new Square[2];
      
      circles[0] = new Circle(1.1, new Point(1, 2), Color.cyan, false);
      circles[1] = new Circle(1.1, new Point(1, 2), Color.cyan, false);
      
      rects[0] = new Rectangle(1, 2, new Point(1, 2), Color.cyan, false);
      rects[1] = new Rectangle(1, 2, new Point(1, 2), Color.cyan, false);
      
      Point a = new Point(1, 1);
      Point b = new Point(0, 2);
      Point c = new Point(0, 0);
      
      tris[0] = new Triangle(a, b, c, Color.cyan, false);
      tris[1] = new Triangle(a, b, c, Color.cyan, false);
      
      Point[] aVertices = new Point[5];
      
      aVertices[0] = new Point(4, 0);
      aVertices[1] = new Point(2, 2);
      aVertices[2] = new Point(-2, -2);
      aVertices[3] = new Point(-4, 0);
      aVertices[4] = new Point(0, -2);

      polys[0] = new ConvexPolygon(aVertices, Color.cyan, false);
      polys[1] = new ConvexPolygon(aVertices, Color.cyan, false);
      
      sqs[0] = new Square(2, new Point (11, 33), Color.cyan, false);
      sqs[1] = new Square(2, new Point (11, 33), Color.cyan, false);
      
      System.out.println("Comparable.compareTo() tests...");
                              
      // Unit test message...
      //
      pass &= test(circles[0].compareTo(circles[1]) == 0, test++);
      pass &= test(rects[0].compareTo(rects[1]) == 0, test++);
      pass &= test(tris[0].compareTo(tris[1]) == 0, test++);
      pass &= test(polys[0].compareTo(polys[1]) == 0, test++);
      pass &= test(sqs[0].compareTo(sqs[1]) == 0, test++);
      
      pass &= test(circles[0].compareTo(polys[0]) < 0, test++);
      pass &= test(polys[0].compareTo(rects[0]) < 0, test++);
      pass &= test(rects[0].compareTo(sqs[0]) < 0, test++);
      pass &= test(sqs[0].compareTo(tris[0]) < 0, test++);
      
      pass &= test(tris[0].compareTo(sqs[0]) > 0, test++);
      pass &= test(sqs[0].compareTo(rects[0]) > 0, test++);
      pass &= test(rects[0].compareTo(polys[0]) > 0, test++);
      pass &= test(polys[0].compareTo(circles[0]) > 0, test++);
      
      // Make some bigger shapes...
      circles[1].setRadius(44.7);
      rects[1].setHeight(12);
      tris[1].setVertexA(new Point(5, 1));
      polys[1].setVertex(0, new Point(8, 0));
      sqs[1].setWidth(4);

      pass &= test(circles[0].compareTo(circles[1]) < 0, test++);
      pass &= test(rects[0].compareTo(rects[1]) < 0, test++);
      pass &= test(tris[0].compareTo(tris[1]) < 0, test++);
      pass &= test(polys[0].compareTo(polys[1]) < 0, test++);
      pass &= test(sqs[0].compareTo(sqs[1]) < 0, test++);

      pass &= test(circles[1].compareTo(circles[0]) > 0, test++);
      pass &= test(rects[1].compareTo(rects[0]) > 0, test++);
      pass &= test(tris[1].compareTo(tris[0]) > 0, test++);
      pass &= test(polys[1].compareTo(polys[0]) > 0, test++);
      pass &= test(sqs[1].compareTo(sqs[0]) > 0, test++);
        
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
         msg = "\nCongratulations, you passed all the tests!\n\n"
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
   
   private static boolean test(boolean pass, int testNum)
   {
      if (!pass)
      {
         System.out.println("   FAILED test #" + testNum);
         System.out.print("   ");
         (new Throwable()).printStackTrace();
      }

      return pass;
   }

   private static boolean approx(double a, double b, double epsilon)
   {
      return Math.abs(a - b) < epsilon;
   }
   
   private static boolean verifyNames(Method[] methods, int modifier, String[] names)
   {
      Arrays.sort(names);
      
      for (Method m : methods)
      {
         if (m.getModifiers() == modifier)
         {
            if (Arrays.binarySearch(names, m.getName()) < 0)
            {
               System.out.println(m.getName());
               return false;
            }
         }
      }
      
      return true;
   }
   
   private static boolean verifyEqualsMethodSignature(Class cl)
   {
      Method[] methods = cl.getDeclaredMethods();
      
      for (Method m : methods)
      {
         if (m.getName().equals("equals"))
         {
            Class<?>[] params = m.getParameterTypes();
            
            if (params.length != 1)
            {
               return false;
            }
            
            if (params[0] != Object.class)
            {
               return false;
            }
            
            return true;
         }
      }
      
      // Missing method, not found...
      return false;
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