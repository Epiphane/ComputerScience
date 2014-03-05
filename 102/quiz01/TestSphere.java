public class TestSphere {
   public static void main(String[] args) {
      Sphere s1 = new Sphere();
      Sphere s2 = new Sphere(0.5);
      Sphere s3 = new Sphere(1.0);
      Sphere s4 = new Sphere(1);
      Sphere s5 = new Sphere(3);
      
      if(!s3.equals(s4)) {
         System.out.println("ERROR: s3 doesn't equal s4");
      }
      if(!s1.equals(s4)) {
         System.out.println("ERROR: s1 doesn't equal s4");
      }
      
      if(s2.getRadius() != 0.5) {
         System.out.println("ERROR getRadius: expected 0.5, got "+s2.getRadius());
      }
      
      if(s2.getDiameter() != s3.getRadius()) {
         System.out.println("ERROR getDiameter: expected 1.0, got "+s2.getDiameter());
      }
      
      if(s5.getVolume() != 36*Math.PI) {
         System.out.println("ERROR getVolume");
      }
      if(s5.getSurfaceArea() != 36*Math.PI) {
         System.out.println("ERROR getSurfaceArea");
      }
      
      System.out.println(s5);
   }
}