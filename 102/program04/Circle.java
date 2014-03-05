/**
  * Circle Class
  * A shape that features a circle centered around
  * one point, with a specific radius.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
import java.awt.*;

public class Circle extends Ellipse {
   
   public Circle(double radius, Point position, Color color, boolean filled) {
      super(radius, radius, position, color, filled);
   }
   
   public double getRadius() {
      return getSemiMajorAxis();
   }
   
   public void setRadius(double radius) {
      super.setSemiMajorAxis(radius);
      super.setSemiMinorAxis(radius);
      super.setSemiMajorAxis(radius);
   }
   
   public void setSemiMinorAxis(double semiMinorAxis) {
      setRadius(semiMinorAxis);
   }
   
   public void setSemiMajorAxis(double semiMajorAxis) {
      setRadius(semiMajorAxis);
   }
}