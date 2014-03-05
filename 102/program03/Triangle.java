/**
  * Triangle Class
  * A shape that features three points.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
import java.awt.*;

public class Triangle implements Shape {
   private Point a, b, c;
   private Color color;
   private boolean filled;
   
   public Triangle(Point a, Point b, Point c, Color color, boolean filled) {
      this.a = a;
      this.b = b;
      this.c = c;
      this.color = color;
      this.filled = filled;
   }
   
   public Point getVertexA() {
      return a;
   }
   
   public void setVertexA(Point a) {
      this.a = a;
   }
   
   public Point getVertexB() {
      return b;
   }
   
   public void setVertexB(Point b) {
      this.b = b;
   }
   
   public Point getVertexC() {
      return c;
   }
   
   public void setVertexC(Point c) {
      this.c = c;
   }
   
   public boolean equals(Object other) {
      if(other == null) {
         return false;
      }
      if(other.getClass() != this.getClass()) {
         return false;
      }
      if(!a.equals(((Triangle)other).a)) {
         return false;
      }
      if(!b.equals(((Triangle)other).b)) {
         return false;
      }
      if(!c.equals(((Triangle)other).c)) {
         return false;
      }
      if(!color.equals(((Triangle)other).color)) {
         return false;
      }
      if(filled != ((Triangle)other).filled) {
         return false;
      }
      
      return true;
   }
   
   public double getArea() {
      double sa = Math.sqrt((a.getX()-b.getX())*(a.getX()-b.getX()) 
                  + (a.getY()-b.getY())*(a.getY()-b.getY()));
      double sb = Math.sqrt((a.getX()-c.getX())*(a.getX()-c.getX())
                  + (a.getY()-c.getY())*(a.getY()-c.getY()));
      double sc = Math.sqrt((b.getX()-c.getX())*(b.getX()-c.getX()) 
                  + (b.getY()-c.getY())*(b.getY()-c.getY()));
      double s = (sa+sb+sc)/2;
      
      return Math.sqrt(s*(s-sa)*(s-sb)*(s-sc));
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
      return a;
   }
   
   public void setPosition(Point position) {
      double dx = position.getX() - a.getX();
      double dy = position.getY() - a.getY();
      move(new Point((int)dx,(int)dy));
   }
   
   public void move(Point delta) {
      movePoint(a, delta);
      movePoint(b, delta);
      movePoint(c, delta);
   }
   
   private void movePoint(Point point, Point delta) {
      double nx = point.getX() + delta.getX();
      double ny = point.getY() + delta.getY();
      
      point.setLocation(nx,ny);
   }
}