/**
  * ComplexPolygon Class
  * A shape that features multiple vertices
  * Arranged in counterclockwise rotation
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
import java.awt.*;

public class ConvexPolygon implements Shape {
   private Point[] vertices;
   private Color color;
   private boolean filled;
   
   public ConvexPolygon(Point[] vertices, Color color, boolean filled) {
      this.vertices = vertices;
      this.color = color;
      this.filled = filled;
   }
   
   public Point getVertex(int index) {
      return vertices[index];
   }
   
   public void setVertex(int index, Point vertex) {
      vertices[index] = vertex;
   }
   
   public boolean equals(Object other) {
      if(other == null) {
         return false;
      }
      if(other.getClass() != this.getClass()) {
         return false;
      }
      if(vertices.length != ((ConvexPolygon)other).vertices.length) {
         return false;
      }
      for(int i = 0; i < vertices.length; i ++) {
         if(!vertices[i].equals(((ConvexPolygon)other).vertices[i])) {
            return false;
         }
      }
      if(!color.equals(((ConvexPolygon)other).color)) {
         return false;
      }
      if(filled != ((ConvexPolygon)other).filled) {
         return false;
      }
      
      return true;
   }
   
   public double getArea() {
      double s1 = 0, s2 = 0;
      for(int i = 0; i < vertices.length; i ++) {
         if(i == vertices.length - 1) {
            s1 += vertices[i].getX() * vertices[0].getY();
            s2 += vertices[i].getY() * vertices[0].getX();
         }
         else {
             s1 += vertices[i].getX() * vertices[i+1].getY();
             s2 += vertices[i].getY() * vertices[i+1].getX();
         }
      }
      
      return (s1 - s2) / 2;
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
      return vertices[0];
   }
   
   public void setPosition(Point position) {
      double dx = position.getX() - vertices[0].getX();
      double dy = position.getY() - vertices[0].getY();
      move(new Point((int)dx,(int)dy));
   }
   
   public void move(Point delta) {
      for(Point vertex : vertices) {
         movePoint(vertex, delta);
      }
   }
   
   private void movePoint(Point point, Point delta) {
      double nx = point.getX() + delta.getX();
      double ny = point.getY() + delta.getY();
      
      point.setLocation(nx,ny);
   }
}