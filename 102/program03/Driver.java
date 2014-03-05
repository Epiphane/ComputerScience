import java.awt.*;

public class Driver {
	public static void main(String[] args) {
		Canvas canvas = new Canvas();
		
		Point[] polygon = new Point[] {new Point(2, 5), new Point(-4,3), new Point(5,1)};
		canvas.add(new ConvexPolygon(polygon, Color.BLUE, true));
		
		System.out.println(canvas.getAreaOfAllShapes());
	}
}