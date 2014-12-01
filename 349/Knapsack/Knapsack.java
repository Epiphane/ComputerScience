import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Comparator;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.lang.Comparable;
import java.util.Collections;
import java.lang.Math;

/*
 * Knapsack.java
 * CPE 349
 * Thomas Steinke
 *
 * 0-1 Knapsack Problem:
 * Given a list of items with their weight and value,
 * Determine the best combination of items to take
 * to maximize the total value (With a maximum capacity limit).
 *
 */

public class Knapsack {
  public static class Item {
    public final int value;
    public final int weight;

    public Item(int value, int weight) {
      this.value = value;
      this.weight = weight;
    }
  }

  public static void main(String[] args) {
    if(args.length == 0) {
      System.out.println("Provide the filename!");
      return;
    }

    // Read in the data
    Scanner input;
    try {
      input = new Scanner(new File(args[0]));
    }
    catch(FileNotFoundException e) {
      e.printStackTrace();
      return;
    }

    // Read in all the items
    int value, weight;
    int inputSize = input.nextInt();
    Item[] items  = new Item[inputSize];
    for(int i = 0; i < inputSize; i ++) {
      input.nextInt();
      value  = input.nextInt();
      weight = input.nextInt();

      items[i] = new Item(value, weight);
    }

    // Read in capacity
    int capacity = input.nextInt();

    long startTime = System.currentTimeMillis();
    if(args.length > 1 && args[1].equals("--enum"))
      printSoln("Using Brute force the best feasible solution found: ",
        items, fullEnumeration(items, capacity));

    printSoln("Greedy solution (not necessarily optimal): ",
      items, greedy(items, capacity));
    printSoln("Dynamic Programming Solution: ",
      items, dynamic(items, capacity));
    long dt = System.currentTimeMillis() - startTime;
    printSoln("Using BnB the best feasible solution found: ",
      items, branchAndBound(items, capacity, dt * 10));
  }

  public static void printSoln(String intro, Item[] items,
    ArrayList<Integer> solution) {

    int weight = 0;
    int value = 0;
    String itemList = "";
    for(Integer item : solution) {
      weight   += items[item].weight;
      value    += items[item].value;
      itemList += (item + 1) + " ";
    }

    System.out.println(intro + value + " " + weight);
    System.out.println(itemList);
  }

  /* BRUTE FORCE SOLUTION */
  public static ArrayList<Integer> fullEnumeration(Item[] items, int C) {
    int bestValue = 0;
    int bestWeight = 0;
    int bestSoln = 0;

    for(int solution = 1; solution < 1 << items.length; solution ++) {
      int weight = 0, value = 0;
      for(int i = 0; i < items.length; i ++) {
        if((solution & (1 << i)) != 0) {
          weight += items[i].weight;
          value  += items[i].value;
        }
      }

      if(weight <= C && value > bestValue) {
        bestValue = value;
        bestWeight = weight;
        bestSoln = solution;
      }
    }

    ArrayList<Integer> result = new ArrayList<Integer>();
    for(int i = 0; i < items.length; i ++) {
      if((bestSoln & (1 << i)) != 0)
        result.add(i);
    }
    return result;
  }

  /* GREEDY ALGORIGTHM */
  public static class ItemEfficiencyComp implements Comparator<Integer> {
    private final Item[] items;

    public ItemEfficiencyComp(Item[] items) { this.items = items; }
    public Integer[] createIndexArray() {
      Integer[] indexes = new Integer[items.length];
      for(int i = 0; i < indexes.length; i ++)
        indexes[i] = i;
      return indexes;
    }

    public int compare(Integer item1, Integer item2) {
      double eff1 = (double) items[item1].value / items[item1].weight;
      double eff2 = (double) items[item2].value / items[item2].weight;
      return eff1 < eff2 ? 1 : -1;
    }
  }

  public static ArrayList<Integer> greedy(Item[] itemsUnsorted, int C) {
    // Order items by value/weight ratio
    Item[] items = itemsUnsorted.clone();

    // Sort the indexes, getting the order from greatest to least
    ItemEfficiencyComp sorter = new ItemEfficiencyComp(items);
    Integer[] indexes = sorter.createIndexArray();
    Arrays.sort(indexes, sorter);

    // Now indexes contains the items with greates ratio to least; be greedy
    int weight = 0, value = 0;
    ArrayList<Integer> result = new ArrayList<Integer>();
    for(int i = 0; i < indexes.length; i ++) {
      if(weight + items[indexes[i]].weight <= C) {
        weight += items[indexes[i]].weight;
        value  += items[indexes[i]].value;
        result.add(indexes[i]);
      }
    }
    Collections.sort(result);

    return result;
  }

  /* DYNAMIC PROGRAMMING ALGORITHM */
  public static ArrayList<Integer> dynamic(Item[] items, int C) {
    int[][] solutions = new int[items.length + 1][C + 1];

    dynamicKnapsack(solutions, items, items.length, C);
    return dynamicBacktrack(solutions, items);
  }

  public static void dynamicKnapsack(int[][] solutions, Item[] items,
    int item, int weight) {

    int leftMax = 0;
    int weightWithoutItem = weight - items[item - 1].weight;
    // Either add this item to a collection of items that fit in
    // A knapsack with W[item] less capacity...
    if(weightWithoutItem >= 0) {
      if(solutions[item - 1][weightWithoutItem] == 0 && item > 1 && weightWithoutItem > 1) {
        dynamicKnapsack(solutions, items, item - 1, weightWithoutItem);
      }

      leftMax = solutions[item - 1][weightWithoutItem] + items[item - 1].value;
    }
    // Or don't add it in, instead fitting something else
    if(solutions[item - 1][weight] == 0 && item > 1)
      dynamicKnapsack(solutions, items, item - 1, weight);

    int rightMax = solutions[item - 1][weight];

    solutions[item][weight] = Math.max(leftMax, rightMax);
  }

  public static ArrayList<Integer> dynamicBacktrack(int[][] solns, Item[] items) {
    int item = solns.length - 1;
    int capacity = solns[0].length - 1;
    ArrayList<Integer> result = new ArrayList<Integer>();

    while(item > 0) {
      if(solns[item][capacity] != solns[item - 1][capacity]) {
        capacity -= items[item - 1].weight;
        result.add(item - 1);
      }
      
      item --;
    }

    Collections.reverse(result);
    return result;
  }

  public static class Leaf implements Comparable<Leaf> {
    public final boolean[] config;
    public int value, weight, bound;

    public Leaf(boolean[] config) { this.config = config; }

    public Leaf child(boolean next, Integer[] indexes, Item[] items, int bound, int C) {
      int nextIndex = config.length;
      boolean[] nextConfig = new boolean[nextIndex + 1];
      System.arraycopy(config, 0, nextConfig, 0, nextIndex);

      nextConfig[nextIndex] = next;
      Leaf child = new Leaf(nextConfig);

      if(next) {
        child.weight = weight + items[indexes[nextIndex]].weight;
        if(child.weight > C) return null;
        child.value = value + items[indexes[nextIndex]].value;
      }
      else {
        child.weight = weight;
        child.value = value;
      }

      // Bound is calculated by (remaining capacity) * (V/W of next best item)
      child.bound = child.value;
      if(nextIndex + 1 < items.length) {
        int nextNextIndex = indexes[nextIndex + 1];
        int remainingCap = C - child.weight;
        child.bound += remainingCap 
          * items[nextNextIndex].value / items[nextNextIndex].weight;
      }
      
      if(bound != -1 && child.bound < bound)
        return null;
      return child;
    }

    public int compareTo(Leaf other) {
      int levelDiff = other.config.length - config.length;
      return levelDiff != 0 ? levelDiff : other.bound - bound;
    }

    public String toString() { 
      String str = "[";
      for(boolean c : config) 
        str += c ? "x," : " ,";
      return str + "]";
    }
  }

  public static ArrayList<Integer> branchAndBound(Item[] itemsUnsorted, int C, long maxTime) {
    long startTime = System.currentTimeMillis();
    // Order items by value/weight ratio
    Item[] items = itemsUnsorted.clone();

    // Sort the indexes, getting the order from greatest to least
    ItemEfficiencyComp sorter = new ItemEfficiencyComp(items);
    Integer[] indexes = sorter.createIndexArray();
    Arrays.sort(indexes, sorter);

    // Create queue with head of tree
    PriorityQueue<Leaf> queue = new PriorityQueue<Leaf>();
    queue.add(new Leaf(new boolean[0]));

    long l = 0;
    Leaf bestSolution = null;
    int bound = -1;
    while(queue.size() > 0) {
      Leaf parent = queue.poll();

      if(parent.config.length == items.length) {
        if(bestSolution == null || parent.value > bestSolution.value) {
          bestSolution = parent;
          bound = parent.value;
        }
      }
      else if(bestSolution == null || bestSolution.value < parent.bound) {
        Leaf trueChild  = parent.child(true, indexes, items, bound, C);
        Leaf falseChild = parent.child(false, indexes, items, bound, C);
        if(trueChild != null)  queue.add(trueChild);
        if(falseChild != null) queue.add(falseChild);
      }

      // Don't go on forever
      if(System.currentTimeMillis() - startTime > maxTime)
        break;
      l ++;
    }

    System.out.println(l);

    ArrayList<Integer> result = new ArrayList<Integer>();
    for(int i = 0; i < items.length; i ++) {
      if(bestSolution.config[i])
        result.add(indexes[i]);
    }
    Collections.sort(result);
    return result;
  }
}
