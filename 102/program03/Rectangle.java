/**
  * Rectangle Class
  * A shape that features 2 sets of parallel lines
  * and 4 right angles. Has a specific width and height,
  * Which are perpendicular.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
import java.awt.*;

public class Rectangle implements Shape {
   private Point position;
   private int width, height;
   private Color color;
   private boolean filled;
   
   public Rectangle(int width, int height, Point position, Color color, boolean filled) {
      this.width = width;
      this.height = height;
      this.position = position;
      this.color = color;
      this.filled = filled;
   }
   
   public int getWidth() {
      return width;
   }
   
   public void setWidth(int width) {
      this.width = width;
   }
   
   public int getHeight() {
      return height;
   }
   
   public void setHeight(int height) {
      this.height = height;
   }
   
   public boolean equals(Object other) {
      if(other == null) {
         return false;
      }
      if(other.getClass() != this.getClass()) {
         return false;
      }
      if(!position.equals(((Rectangle)other).position)) {
         return false;
      }
      if(width != ((Rectangle)other).width) {
         return false;
      }
      if(height != ((Rectangle)other).height) {
          return false;
       }
      if(!color.equals(((Rectangle)other).color)) {
         return false;
      }
      if(filled != ((Rectangle)other).filled) {
         return false;
      }
      
      return true;
   }
   
   public double getArea() {
      return width*height;
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