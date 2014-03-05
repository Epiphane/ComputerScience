/**
  * Circle Class
  * A shape that features a circle centered around
  * one point, with a specific radius.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
import java.awt.*;

public class Circle implements Shape {
   private double radius;
   private Point position;
   private Color color;
   private boolean filled;
   
   public Circle(double radius, Point position, Color color, boolean filled) {
      this.radius = radius;
      this.position = position;
      this.color = color;
      this.filled = filled;
   }
   
   public double getRadius() {
      return radius;
   }
   
   public void setRadius(double radius) {
      this.radius = radius;
   }
   
   public boolean equals(Object other) {
      if(other == null) {
         return false;
      }
      if(other.getClass() != this.getClass()) {
         return false;
      }
      if(!position.equals(((Circle)other).position)) {
         return false;
      }
      if(radius != ((Circle)other).radius) {
         return false;
      }
      if(!color.equals(((Circle)other).color)) {
         return false;
      }
      if(filled != ((Circle)other).filled) {
         return false;
      }
      
      return true;
   }
   
   public double getArea() {
      return Math.PI * radius * radius;
   }
   
   public Color getColor() {
      return color;
   }
   
   public void setColor(Color color) {
      this.color = color;
   }
      
   public boolean getFilled() {
      return filled;
   }
   
   public void setFilled(boolean filled) {
      this.filled = filled;
   }
   
   public Point getPosition() {
      return position;
   }
   
   public void setPosition(Point position) {
      this.position = position;
   }
   
   public void move(Point delta) {
      double nx = position.getX() + delta.getX();
      double ny = position.getY() + delta.getY();
      position.setLocation(nx, ny);
   }
}