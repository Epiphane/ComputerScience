import java.util.Scanner;
import java.util.ArrayList;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * BipartiteCheck.java
 *
 * Checks whether a given graph is bipartite
 *
 * @author tsteinke
 *
 */

public class BipartiteCheck {
  public static void main(String[] args) {
    if(args.length == 0) {
      System.out.println("Provide the filename!");
      return;
    }

    // Create graph
    GraphStart graph = new GraphStart();
    try {
      graph.readfile_graph(args[0]);
    }
    catch(FileNotFoundException e) {
      e.printStackTrace();
      return;
    }
    System.out.print("Graph has (" + graph.nvertices + " vertices, " + graph.nedges + " edges) and is ");

    // Now for the bipartite algorithm
    boolean bipartite = isBipartite(graph);
    if(!bipartite)
      System.out.print("NOT ");
    System.out.println("BiColorable");

    ConnectCheck.outputConnectedComponents(graph);
  }

  /*
   * Determines whether a given adjacency matrix is bipartite
   */
  public static boolean isBipartite(GraphStart graph) {
    int colors[] = new int[graph.nvertices]; // 0: not visited
                                             // 1-2: Bipartite colors

    int firstVertex = 0;
    while(firstVertex < graph.nvertices) {
      // Go through all vertices, if it hasn't been colored start a search
      if(colors[firstVertex] == 0) {
        colors[firstVertex] = 1;
        if(!isBipartiteDFS(graph, colors, firstVertex)) {
          return false;
        }
      }

      firstVertex ++;
    }

    // No false no problem
    return true;
  }

  public static boolean isBipartiteDFS(GraphStart graph, int[] colors, int vertex) {
    // Look for all connections
    ArrayList<Integer> edges = graph.edges[vertex + 1];

    // Check them all for unvisited-ness
    for(int edge : edges) {
      if(colors[edge - 1] == 0) {
        colors[edge - 1] = 3 - colors[vertex];

        if(!isBipartiteDFS(graph, colors, edge - 1)) {
          // If we get a false, then game over
          return false;
        }
      }
      // If it's the same color as us, that's no good
      else if(colors[vertex] == colors[edge - 1]) {
        return false;
      }
    }

    // No problems? Then we're good
    return true;
  }
}
