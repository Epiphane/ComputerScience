/**
  * ComplexPolygon Class
  * A shape that features multiple vertices
  * Arranged in counterclockwise rotation
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
import java.awt.*;

public class ConvexPolygon extends Shape {
   private Point[] vertices;
   
   public ConvexPolygon(Point[] vertices, Color color, boolean filled) {
      super(color, filled);
      this.vertices = new Point[vertices.length];
      for(int i = 0; i < vertices.length; i ++) {
         this.vertices[i] = new Point((int)vertices[i].getX(),(int)vertices[i].getY());
      }
   }
   
   public Point getVertex(int index) {
      return new Point((int)vertices[index].getX(), (int)vertices[index].getY());
   }
   
   public void setVertex(int index, Point vertex) {
      vertices[index] = new Point((int)vertex.getX(),(int)vertex.getY());
   }
   
   public boolean equals(Object other) {
      if(!super.equals(other)) {
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
      
      if(s1 >= s2) {
         return (s1 - s2) / 2;
      }
      else {
         return (s2 - s1) / 2;
      }
}
   
   public Point getPosition() {
      return getVertex(0);
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