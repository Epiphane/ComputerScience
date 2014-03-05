/**
  * Triangle Class
  * A shape that features three points.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
import java.awt.*;

public class Triangle extends ConvexPolygon {
   
   public Triangle(Point a, Point b, Point c, Color color, boolean filled) {
      super(new Point[] {a, b, c}, color, filled);
   }
   
   public Point getVertexA() {
      return getVertex(0);
   }
   
   public void setVertexA(Point a) {
      setVertex(0, a);
   }
   
   public Point getVertexB() {
      return getVertex(1);
   }
   
   public void setVertexB(Point b) {
      setVertex(1, b);
   }
   
   public Point getVertexC() {
      return getVertex(2);
   }
   
   public void setVertexC(Point c) {
      setVertex(2, c);
   }
}