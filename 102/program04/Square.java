/**
  * Square Class
  * A shape that features 2 sets of parallel lines
  * and 4 right angles. All lines are the same length.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
import java.awt.*;

public class Square extends Rectangle {
   public Square(int sideLength, Point position, Color color, boolean filled) {
      super(sideLength, sideLength, position, color, filled);
   }
   
   public int getSize() {
      return getWidth();
   }
   
   public void setSize(int size) {
      super.setWidth(size);
      super.setHeight(size);
   }
   
   public void setWidth(int width) {
      setSize(width);
   }
   
   public void setHeight(int height) {
      setSize(height);
   }
}