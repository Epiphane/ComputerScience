import java.awt.*;
import java.lang.Comparable;
/**
  * Shape Interface
  * An interface with all the important functionality of Shapes.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
public abstract class Shape implements java.lang.Comparable<Shape> {
   private Color color;
   private boolean filled;
   
   /**
    * Initializes the Shape class with a specific color and filled state.
    * 
    * @param Color representing the Shape's color
    * @param boolean representing whether or not the Shape is filled
    */
   public Shape(Color color, boolean filled) {
      this.color = color;
      this.filled = filled;
   }
   /**
    * This method calculates and returns the area of the shape.
    *
    * @return A double representing the total area of the shape.
    */
   public abstract double getArea();
   
   /**
    * This method returns the color of the shape.
    *
    * @return A java.awt.Color representing the color of the shape.
    */
   public Color getColor() {
      return color;
   }
   
   /**
    * This method changes the color of the shape.
    *
    * @param color representing the color of the shape.
    */
   public void setColor(Color color) {
      this.color = color;
   }
      
   /**
    * This method determines and returns whether or not the shape is filled.
    *
    * @return A boolean representing whether or not the shape is filled.
    * True if filled, false if not.
    */
   public boolean getFilled() {
      return filled;
   }
   
   /**
    * This method changes whether or not the shape is filled.
    *
    * @param filled - true if it should be filled, false if it should not be filled
    */
   public void setFilled(boolean filled) {
      this.filled = filled;
   }
   
   /**
    * This method returns the position of the shape
    *
    * @return A java.awt.Point representing the current position of the shape.
    */
   public abstract Point getPosition();
   
   /**
    * This method sets the shape to a new position
    *
    * @param position representing where to move the shape
    */
   public abstract void setPosition(Point position);
   
   /**
    * This method moves the shape a specified distance
    *
    * @param delta representing the distance to move in the x and y position
    */
   public abstract void move(Point delta);
   
   public int compareTo(Shape o) {
      if(this.getClass().getName().compareTo(o.getClass().getName()) != 0) {
         return this.getClass().getName().compareTo(o.getClass().getName());
      }
      if(getArea() < o.getArea()) {
         return -1;
      }
      if(getArea() > o.getArea()) {
          return 1;
      }
      return 0;
   }
   
   public boolean equals(Object o) {
      if(o == null) {
         return false;
      }
      if(o.getClass() != this.getClass()) {
         return false;
      }
      if(!color.equals(((Shape)o).color)) {
         return false;
      }
      if(filled != ((Shape)o).filled) {
          return false;
       }
      return true;
   }
}