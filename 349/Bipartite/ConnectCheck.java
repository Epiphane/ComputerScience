import java.util.ArrayList;

/**
 * ConnectCheck.java
 *
 * Checks connectivity of a graph
 *
 * @author tsteinke
 *
 */

public class ConnectCheck {
  public static void outputConnectedComponents(boolean[][] vertices) {
    int[] vertexComponents = new int[vertices.length];

    int firstVertex = 0;
    int numComponents = 0;
    while(firstVertex < vertices.length) {
      // Go through all vertices, if it hasn't been colored start a search
      if(vertexComponents[firstVertex] == 0) {
        numComponents ++;
        vertexComponents[firstVertex] = numComponents;
        connectComponentDFS(vertices, vertexComponents, firstVertex);
      }

      firstVertex ++;
    }

    System.out.println("It has " + numComponents + " connected components:");
    for(int component = 0; component < numComponents; component ++) {
      System.out.print("{ ");
      boolean firstNode = true;
      for(int ndx = 0; ndx < vertices.length; ndx ++) {
        if(vertexComponents[ndx] - 1 == component) {
          if(firstNode) {
            firstNode = false;
          }
          else {
            System.out.print(", ");
          }

          System.out.print(ndx + 1);
        }
      }
      System.out.println(" }");
    }
  }

  public static void connectComponentDFS(boolean[][] vertices, int[] vertexComponents, int vertex) {
    // Look for all connections
    for(int ndx = 0; ndx < vertices.length; ndx ++) {
      if(vertices[vertex][ndx]) {
        // If it's not in a component, take it over
        if(vertexComponents[ndx] == 0) {
          vertexComponents[ndx] = vertexComponents[vertex];
          connectComponentDFS(vertices, vertexComponents, ndx);
        }
      }
    }
  }
}
