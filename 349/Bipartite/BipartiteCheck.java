import java.util.Scanner;
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

    Scanner input;
    try {
      input = new Scanner(new File(args[0]));
    }
    catch(FileNotFoundException e) {
      System.out.println("File not found!");
      return;
    }

    // Begin actual program
    // Figure out directedness. 1 = directed, 0 = undirected
    boolean directed = (input.nextInt() == 1);

    // Next, number of vertices/edges
    int numVertices = input.nextInt();
    int numEdges = input.nextInt();
    System.out.print("Graph has (" + numVertices + " vertices, " + numEdges + " edges) and is ");

    // Init empty graph
    boolean[][] vertices = new boolean[numVertices][numVertices];

    // And read in points for it
    while(numEdges-- > 0) {
      int i = input.nextInt() - 1;
      int j = input.nextInt() - 1;

      vertices[i][j] = vertices[j][i] = true;
    }

    // Now for the bipartite algorithm
    boolean bipartite = isBipartite(vertices);
    if(!bipartite)
      System.out.print("NOT ");
    System.out.println("BiColorable");

    ConnectCheck.outputConnectedComponents(vertices);
  }

  /*
   * Determines whether a given adjacency matrix is bipartite
   */
  public static boolean isBipartite(boolean[][] vertices) {
    int colors[] = new int[vertices.length]; // 0: not visited
                                             // 1-2: Bipartite colors

    int firstVertex = 0;
    while(firstVertex < vertices.length) {
      // Go through all vertices, if it hasn't been colored start a search
      if(colors[firstVertex] == 0) {
        colors[firstVertex] = 1;
        if(!isBipartiteDFS(vertices, colors, firstVertex)) {
          return false;
        }
      }

      firstVertex ++;
    }

    // No false no problem
    return true;
  }

  public static boolean isBipartiteDFS(boolean[][] vertices, int[] colors, int vertex) {
    for(int ndx = 0; ndx < vertices.length; ndx ++) {
      // Test for connection
      if(vertices[vertex][ndx]) {
        // If color is 0, we need to visit it
        if(colors[ndx] == 0) {
          // We need to go deeper
          // Set the color first
          colors[ndx] = 3 - colors[vertex];
          if(!isBipartiteDFS(vertices, colors, ndx)) {
            // If we get a false, then game over
            return false;
          }
        }

        // If it's the same color as us, that's no good
        else if(colors[vertex] == colors[ndx]) {
          return false;
        }
        // else: it's bipartite
      }
    }

    // No problems? Then we're good
    return true;
  }
}
