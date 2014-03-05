/**
  * Circle Class
  * A shape that features a circle centered around
  * one point, with a specific radius.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
import java.awt.*;

public class Ellipse extends Shape {
   private double semiMajorAxis, semiMinorAxis;
   private Point position;
   
   public Ellipse(double semiMajorAxis, double semiMinorAxis, Point position, 
                  Color color, boolean filled) {
      super(color, filled);
      this.semiMajorAxis = semiMajorAxis;
      this.semiMinorAxis = semiMinorAxis;
      this.position = new Point((int)position.getX(), (int)position.getY());
   }
   
   public double getSemiMajorAxis() {
      return semiMajorAxis;
   }
   
   public void setSemiMajorAxis(double semiMajorAxis) {
      this.semiMajorAxis = semiMajorAxis;
      if(this.semiMajorAxis < this.semiMinorAxis) {
         double tAxis = this.semiMajorAxis;
         this.semiMajorAxis = this.semiMinorAxis;
         this.semiMinorAxis = tAxis;
      }
   }
   
   public double getSemiMinorAxis() {
      return semiMinorAxis;
   }
   
   public void setSemiMinorAxis(double semiMinorAxis) {
      this.semiMinorAxis = semiMinorAxis;
      if(this.semiMajorAxis < this.semiMinorAxis) {
         double tAxis = this.semiMajorAxis;
         this.semiMajorAxis = this.semiMinorAxis;
         this.semiMinorAxis = tAxis;
      }
   }
   
   public boolean equals(Object other) {
      if(!super.equals(other)) {
         return false;
      }
      if(semiMinorAxis != ((Ellipse)other).semiMinorAxis) {
         return false;
      }
      if(semiMajorAxis != ((Ellipse)other).semiMajorAxis) {
         return false;
      }
      if(!position.equals(((Ellipse)other).position)) {
         return false;
      }
      
      return true;
   }
   
   public double getArea() {
      return Math.PI * semiMajorAxis * semiMinorAxis;
   }
      
   public Point getPosition() {
      return new Point((int)position.getX(), (int)position.getY());
   }
   
   public void setPosition(Point position) {
      this.position = new Point((int)position.getX(), (int)position.getY());
   }
   
   public void move(Point delta) {
      double nx = position.getX() + delta.getX();
      double ny = position.getY() + delta.getY();
      position.setLocation(nx, ny);
   }
}