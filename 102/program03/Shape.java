/**
  * Shape Interface
  * An interface with all the important functionality of Shapes.
  *
  * @author Thomas Steinke
  * @version Program 3
  */
  
public interface Shape {
   /**
    * This method calculates and returns the area of the shape.
    *
    * @return A double representing the total area of the shape.
    */
   public double getArea();
   
   /**
    * This method returns the color of the shape.
    *
    * @return A java.awt.Color representing the color of the shape.
    */
   public java.awt.Color getColor();
   
   /**
    * This method changes the color of the shape.
    *
    * @param color representing the color of the shape.
    */
   public void setColor(java.awt.Color color);
      
   /**
    * This method determines and returns whether or not the shape is filled.
    *
    * @return A boolean representing whether or not the shape is filled.
    * True if filled, false if not.
    */
   public boolean getFilled();
   
   /**
    * This method changes whether or not the shape is filled.
    *
    * @param filled - true if it should be filled, false if it should not be filled
    */
   public void setFilled(boolean filled);
   
   /**
    * This method returns the position of the shape
    *
    * @return A java.awt.Point representing the current position of the shape.
    */
   public java.awt.Point getPosition();
   
   /**
    * This method sets the shape to a new position
    *
    * @param position representing where to move the shape
    */
   public void setPosition(java.awt.Point position);
   
   /**
    * This method moves the shape a specified distance
    *
    * @param delta representing the distance to move in the x and y position
    */
   public void move(java.awt.Point delta);
}