/**
 * @author Thomas Steinke
 * @version Lab Quiz 1
 */

public class Sphere {
   private double r;
   
   public Sphere() {
      r = 1.0;
   }
   
   public Sphere(double radius) {
      r = radius;
   }
   
   public boolean equals(Sphere s) {
      if(s == null) {
         return false;
      }
      
      if(s.getRadius() == r) {
         return true;
      }
      return false;
   }
   
   public double getRadius() {
      return r;
   }
   
   public double getDiameter() {
      return r*2;
   }
   
   public double getSurfaceArea() {
      return 4*Math.PI*r*r;
   }
   
   public double getVolume() {
      return 4*Math.PI*r*r*r/3;
   }
   
   public String toString() {
      return "The Sphere class is immutable!";
   }
}