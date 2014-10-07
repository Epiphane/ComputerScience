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
  public static void outputConnectedComponents(GraphStart graph) {
    int[] vertexComponents = new int[graph.nvertices];

    int firstVertex = 0;
    int numComponents = 0;
    while(firstVertex < graph.nvertices) {
      // Go through all vertices, if it hasn't been colored start a search
      if(vertexComponents[firstVertex] == 0) {
        numComponents ++;
        vertexComponents[firstVertex] = numComponents;
        connectComponentDFS(graph, vertexComponents, firstVertex);
      }

      firstVertex ++;
    }

    System.out.println("It has " + numComponents + " connected components:");
    for(int component = 0; component < numComponents; component ++) {
      System.out.print("{ ");
      boolean firstNode = true;
      for(int ndx = 0; ndx < graph.nvertices; ndx ++) {
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

  public static void connectComponentDFS(GraphStart graph, int[] vertexComponents, int vertex) {
    // Look for all connections
    ArrayList<Integer> edges = graph.edges[vertex + 1];

    // Check them all for unvisited-ness
    for(int edge : edges) {
      if(vertexComponents[edge - 1] == 0) {
        vertexComponents[edge - 1] = vertexComponents[vertex];
        connectComponentDFS(graph, vertexComponents, edge - 1);
      }
    }
  }
}
