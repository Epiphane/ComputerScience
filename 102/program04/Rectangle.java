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

public class Rectangle extends ConvexPolygon {
   public Rectangle(int width, int height, Point a, Color color, boolean filled) {
      super(new Point[] {a, new Point((int)a.getX()+width, (int)a.getY()),
                         new Point((int)a.getX()+width, (int)a.getY()-height),
                         new Point((int)a.getX(), (int)a.getY()-height)}, color, filled);
   }
   
   public int getWidth() {
      return (int) (getVertex(1).getX() - getVertex(0).getX());
   }
   
   public void setWidth(int width) {
      super.setVertex(1, new Point((int)getVertex(0).getX()+width, (int)getVertex(0).getY()));
      super.setVertex(2, new Point((int)getVertex(3).getX()+width, (int)getVertex(3).getY()));
   }
   
   public int getHeight() {
      return (int) (getVertex(0).getY() - getVertex(3).getY());
   }
   
   public void setHeight(int height) {
      super.setVertex(3, new Point((int)getVertex(0).getX(), (int)getVertex(0).getY()-height));
      super.setVertex(2, new Point((int)getVertex(1).getX(), (int)getVertex(1).getY()-height));
   }
   
   public void setVertex(int index, Point vertex) {
      throw new UnsupportedOperationException();
   }
}