import java.util.ArrayList;
import java.io.FileNotFoundException;

/**
 * TopologicalSort.java
 *
 * Checks whether a given topological sort is correct
 *
 * @author tsteinke
 *
 */

public class TopologicalSort {
  public static void main(String[] args) {
    if(args.length == 0) {
      System.out.println("Provide the filename!");
      return;
    }

    // Create graph
    DAG graph = new DAG();
    try {
      graph.readfile_graph(args[0]);
    }
    catch(FileNotFoundException e) {
      e.printStackTrace();
      return;
    }

    for(int i = 0; i < graph.nvertices; i ++) {
      System.out.print(graph.topology[i] + " ");
    }
    System.out.print("is ");

    // Run the algorithm!
    if(!validTopologicalSort(graph, graph.topology))
      System.out.print("NOT ");

    System.out.println("a valid topological sort for the given graph");
  }

  public static boolean validTopologicalSort(DAG graph, int[] topology) {
    // Verify each vertex's n-index
    for(int i = 1; i < topology.length; i ++) {
      // If it's not degree 0, game over
      if(graph.degree[topology[i]] > 0) {
        return false;
      }

      // Otherwise keep walking
      ArrayList<Integer> edges = graph.edges[i];
      while(edges.size() > 0) {
        graph.remove_edge(i, edges.get(0));
      }
    }

    return true;
  }
}
