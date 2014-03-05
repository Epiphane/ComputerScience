/**
  * Canvas Class
  * Holds an ArrayList with shapes.
  * Allows shapes to be added, modified, and removed,
  * As well as a variety of analytical methods.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
import java.awt.*;
import java.util.ArrayList;

public class Canvas {
   private ArrayList<Shape> shapeList;

   /**
    * Default constructor for the Canvas class
    */
   public Canvas() {
      shapeList = new ArrayList<Shape>();
   }

   /**
    * Adds a shape to the canvas
    *
    * @param Shape referencing a shape to be added to the canvas
    */
   public void add(Shape shape) {
      shapeList.add(shape);
   }

   /**
    * Remove a shape from the canvas
    *
    * @param int - the index in shapeList of the Shape to be removed
    */
   public Shape remove(int index) {
      try {
         return shapeList.remove(index);
      }
      catch(Exception e) {
         return null;
      }
   }

   /**
    * Gets a Shape from the canvas
    *
    * @param int - the index in shapeList of the Shape to be retrieved
    * @return Shape object that has been retrieved from the Canvas
    */
   public Shape get(int index) {
      try {
          return shapeList.get(index);
       }
       catch(Exception e) {
          return null;
       }
   }

   /**
    * Returns the amount of Shapes on the Canvas
    *
    * @return int - the amount of shapes on the Canvas
    */
   public int size() {
       return shapeList.size();
   }

   /**
    * Gets a list of all the circles on the canvas
    *
    * @return ArrayList<Circle> containing all the Circle objects on the Canvas
    */
   public ArrayList<Circle> getCircles() {
      ArrayList<Circle> circles = new ArrayList<Circle>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i) instanceof Circle) {
            circles.add((Circle)shapeList.get(i));
         }
      }
   
      return circles;
   }

   /**
    * Gets a list of all the Rectangles on the canvas
    *
    * @return ArrayList<Rectangle> containing all the Rectangle objects on the Canvas
    */
   public ArrayList<Rectangle> getRectangles() {
      ArrayList<Rectangle> rectangles = new ArrayList<Rectangle>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i).getClass() == Rectangle.class) {
            rectangles.add((Rectangle)shapeList.get(i));
         }
      }
   
      return rectangles;
   }

   /**
    * Gets a list of all the Triangles on the canvas
    *
    * @return ArrayList<Triangle> containing all the Triangle objects on the Canvas
    */
   public ArrayList<Triangle> getTriangles() {
      ArrayList<Triangle> triangles = new ArrayList<Triangle>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i) instanceof Triangle) {
            triangles.add((Triangle)shapeList.get(i));
         }
      }
   
      return triangles;
   }

   /**
    * Gets a list of all the ConvexPolygons on the canvas
    *
    * @return ArrayList<ConvexPolygon> containing all the ConvexPolygon objects on the Canvas
    */
   public ArrayList<ConvexPolygon> getConvexPolygons() {
      ArrayList<ConvexPolygon> convexPolygons = new ArrayList<ConvexPolygon>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i).getClass() == ConvexPolygon.class) {
            convexPolygons.add((ConvexPolygon)shapeList.get(i));
         }
      }
   
      return convexPolygons;
   }

   /**
    * Gets a list of all the Shapes of a certain color
    *
    * @param Color representing the color of every shape you want to get.
    * @return ArrayList<Shape> containing all the Shape objects of that color
    */
   public ArrayList<Shape> getShapesByColor(Color color) {
      ArrayList<Shape> shapesOfColor = new ArrayList<Shape>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i).getColor().equals(color)) {
            shapesOfColor.add(shapeList.get(i));
         }
      }
   
      return shapesOfColor;
   }

   /**
    * Gets the area of all the shapes on the Canvas
    *
    * @return double representing the combined area of all shapes
    */
   public double getAreaOfAllShapes() {
      double totalArea = 0;
      for(int i = 0; i < shapeList.size(); i ++) {
         totalArea += shapeList.get(i).getArea();
      }
      
      return totalArea;
   }
}