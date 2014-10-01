import java.util.ArrayList;

/**
 * LexPermGenerator.java
 *
 * Generates lexigraphic-ordered permutations
 * of a string.
 *
 * Based on pseudocode from the handout.
 *
 * @author tsteinke
 *
 */

public class LexPermGenerator {
  public static ArrayList<String> getPermutations(String input) {
    ArrayList<String> permutations = new ArrayList<String>();

    // Base case is empty string
    // Return it
    if(input.length() == 0) {
      permutations.add("");
      return permutations;
    }

    // Otherwise take all letters off one by one and add them to the front
    // So abc -> a (pass on bc)
    //           b (pass on ac)
    //           c (pass on ab)
    for(int letterToAppendToFront = 0; letterToAppendToFront < input.length(); letterToAppendToFront ++) {
      char toPutAtFront = input.charAt(letterToAppendToFront);

      ArrayList<String> smallPerms;
      // Edge case test!
      if(letterToAppendToFront == 0) {
        smallPerms = getPermutations(input.substring(1));
      }
      else if(letterToAppendToFront == input.length() - 1) {
        smallPerms = getPermutations(input.substring(0, letterToAppendToFront));
      }
      else {
        smallPerms = getPermutations(input.substring(0, letterToAppendToFront) + input.substring(letterToAppendToFront + 1));
      }

      // If we get results back, add them
      for(String smallPerm : smallPerms) {
        permutations.add(toPutAtFront + smallPerm);
      }
    }

    return permutations;
  }
}
