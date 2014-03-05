import java.util.*;
import java.io.*;

public class Dictionary implements java.lang.Iterable<String> {
   private ArrayList<String> wordList;

   public Dictionary(String fileName, boolean sort) throws DictionaryException {
      try {
         wordList = new ArrayList<String>();
         
         Scanner inFile = new Scanner(new File(fileName));
      
         while(inFile.hasNextLine()) {
            wordList.add(inFile.nextLine());
         }
      
         if(sort) {
            sort();
         }
      }
      catch(Exception e) {
         throw new DictionaryException("Error creating Dictionary");
      }
   }
   
   public Iterator<String> iterator() {
      return wordList.iterator();
   }
   
   public boolean lookUp(String word) {
      int min = 0;
      int max = wordList.size() - 1;

      while (min <= max) {
         int mid = (min + max) / 2;
             
         if (word.compareTo(wordList.get(mid)) < 0) {
            max = mid - 1;
         }
         else if (word.compareTo(wordList.get(mid)) > 0) {
            min = mid + 1;
         }
         else {
            return true;
         }
      }

      return false;
   }
   
   public void write(String fileName) throws DictionaryException {
      try {
          PrintStream file = new PrintStream(new File(fileName));
       
          for(int i = 0; i < wordList.size(); i ++) {
             file.println(wordList.get(i));
          }
       }
       catch(Exception e) {
          throw new DictionaryException("Error writing Dictionary");
       }
   }

   private void sort() {
      for (int i = 0; i < wordList.size() - 1; i++) {
         int minPos = minimumPosition(i);
         swap(minPos, i);
      }
   }
   
   private int minimumPosition(int from) {
      int minPos = from;

      for (int i = from + 1; i < wordList.size(); i++) {
         if (wordList.get(i).compareTo(wordList.get(minPos)) < 0) {
            minPos = i;
         }
      }

      return minPos;
   }

   private void swap(int i, int j) {
      String temp = wordList.get(i);
      wordList.set(i, wordList.get(j));
      wordList.set(j, temp);
   }
}