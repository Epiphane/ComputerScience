import java.util.ArrayList;

/**
 * MinChangePermGenerator.java
 *
 * Generates minimum-change-ordered permutations
 * of a string.
 *
 * Based on pseudocode from the textbook.
 *
 * @author tsteinke
 *
 */

public class MinChangePermGenerator {
  public static ArrayList<String> getPermutations(String input) {
    ArrayList<String> permutations = new ArrayList<String>();

    // Base case: return string
    if(input.length() == 1) {
      permutations.add(input);
      return permutations;
    }

    // Otherwise, we take off the first letter and get permutations of n-1 letters
    ArrayList<String> smallPerms = getPermutations(input.substring(0, input.length() - 1));
    char charToAdd = input.charAt(input.length() - 1);

    // Cycle through the given permutations and for each, insert R->L then L->R etc
    int direction = -1;
    int ndxInPerm;
    for(String permutation : smallPerms) {
      // Set starting place based on the direction you go
      if(direction == -1)
        ndxInPerm = permutation.length();
      else
        ndxInPerm = 0;

      // Move through string now!
      while(ndxInPerm >= 0 && ndxInPerm <= permutation.length()) {
        permutations.add(permutation.substring(0, ndxInPerm) + charToAdd + permutation.substring(ndxInPerm));

        ndxInPerm += direction;
      }

      direction *= -1;
    }

    return permutations;
  }
}
