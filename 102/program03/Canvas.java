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
   
   public Canvas() {
      shapeList = new ArrayList<Shape>();
   }
   
   public void add(Shape shape) {
      shapeList.add(shape);
   }
   
   public Shape remove(int index) {
      try {
         return shapeList.remove(index);
      }
      catch(Exception e) {
         return null;
      }
   }
   
   public Shape get(int index) {
      try {
          return shapeList.get(index);
       }
       catch(Exception e) {
          return null;
       }
   }
   
   public int size() {
       return shapeList.size();
   }
   
   public ArrayList<Circle> getCircles() {
      ArrayList<Circle> circles = new ArrayList<Circle>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i) instanceof Circle) {
            circles.add((Circle)shapeList.get(i));
         }
      }
   
      return circles;
   }
   
   public ArrayList<Rectangle> getRectangles() {
      ArrayList<Rectangle> rectangles = new ArrayList<Rectangle>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i) instanceof Rectangle) {
            rectangles.add((Rectangle)shapeList.get(i));
         }
      }
   
      return rectangles;
   }
   
   public ArrayList<Triangle> getTriangles() {
      ArrayList<Triangle> triangles = new ArrayList<Triangle>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i) instanceof Triangle) {
            triangles.add((Triangle)shapeList.get(i));
         }
      }
   
      return triangles;
   }
   
   public ArrayList<ConvexPolygon> getConvexPolygons() {
      ArrayList<ConvexPolygon> convexPolygons = new ArrayList<ConvexPolygon>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i) instanceof ConvexPolygon) {
            convexPolygons.add((ConvexPolygon)shapeList.get(i));
         }
      }
   
      return convexPolygons;
   }
   
   public ArrayList<Shape> getShapesByColor(Color color) {
      ArrayList<Shape> shapesOfColor = new ArrayList<Shape>();
      for(int i = 0; i < shapeList.size(); i ++) {
         if(shapeList.get(i).getColor().equals(color)) {
            shapesOfColor.add(shapeList.get(i));
         }
      }
   
      return shapesOfColor;
   }
   
   public double getAreaOfAllShapes() {
      double totalArea = 0;
      for(int i = 0; i < shapeList.size(); i ++) {
         totalArea += shapeList.get(i).getArea();
      }
      
      return totalArea;
   }
}