/****
 * @author efiske, tsteinke, Elliot Fiske, Thomas Steinke
 * 
 * Class HashTable implements a HashTable ADT for any generic hashable object type T.
 * It provides utilities to iterate through the hashtable as well as to find, delete, and insert any element in O(1) time.
 */

import java.util.Iterator;
import java.util.NoSuchElementException;

public class HashTable {
	/****
	 * HashEntry stores the current active or inactive state of this entry as well
	 * as the element it contains.
	 */
	private class HashEntry {
		/** Active is true if this Hash Entry is available to enter an element into. 
		 * Active is set to false if SOMETHING... WHAT?? */
		public boolean active;
		public Object element;

		public HashEntry(Object element) {
			active = true;
			this.element = element;
		}
	}

	private HashEntry[] entries;
	/** The number of active AND inactive cells in the table. */ 
	private int occupiedCells;

	/** Intialize a new HashTable with the specified number of elements in the
	 * collection.
	 * @param numElements How many elements are in the collection when this table is created.
	 */
	public HashTable(int numElements) {
		//Set table size to the first prime number that is >= to numElements*2
		entries = new HashEntry[nextPrime(numElements*2)];
		occupiedCells = 0;
	}

	/** Create a unique "hash" for an Object for the array
	 * @param x An object that needs a hash
	 * @return A new hash for the specified Object */
	private int hash(Object x) {
		return Math.abs(x.hashCode()) % entries.length;
	}

	/**** Class Iter provides functionality to iterate through the hash table. */
	private class Iter implements Iterator {

		/** The current position of the iterator in the array. */
		private int cursor = 0;

		@Override
		public boolean hasNext() {
			//Don't modify the real cursor when we're just testing hasNext
			int tempCursor = cursor + 1;
			
			//Iterate through the hashtable
			while(tempCursor < entries.length - 1) {
				tempCursor++;
				//Looks for the next non-null, active entry
				if(entries[tempCursor] == null || !entries[tempCursor].active) continue;
				
				return true;
			}
			
			//No valid entry was found and we reached the end. False it is!
			return false;
		}

		@Override
		public Object next() {
			while(cursor <= entries.length - 1) {
				if(entries[cursor] == null || !entries[cursor].active) {
					cursor++;
					continue;
				}
				
				return entries[cursor++].element;
			}
			
			//We reached the end without finding a valid element. Must be done.
			throw new NoSuchElementException();
		}

		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}
	}


	/** Given an integer value, returns the closest prime value greater than
	 * that number.
	 * @param currPrime The lower exclusive bound of our prime search
	 */
	private int nextPrime(int currPrime) {
		//Handle small numbers
		if(currPrime < 2) return 2;

		//Go up till you find the next prime number
		while(!isPrime(++currPrime));

		return currPrime;
	}

	/**
	 * @param n An integer whose primality shall be tested
	 * @return True if the integer is prime, false if the integer is composite.
	 */
	private boolean isPrime(int n) {
		//Handle small numbers
		if(n < 2) return false;
		if(n == 2 || n == 3) return true;

		//Handle bigger numbers
		for(int i = 2; i <= n/2; i++)
			if(n % i == 0) return false;

		return true;
	}

	/**
	 * Helper method that returns the appropriate index of an object.
	 * @param x The object that we need an index for
	 * @return The index that would be appropriate for this object in the hash table.
	 */
	private int findPosition(Object x) {
		/** The probe searches the hash table until it finds an appropriate 
		 * place for x, by incrementing by i^2 each time */
		int i = 0;
		/** The original position that x would hash to. */
		int hashValue = hash(x);
		/** The index identifies the cell to be probed */
		int index = hashValue;

		//Go through the array and find a valid entry location for x
		while(entries[index] != null && entries[index].element.hashCode() != x.hashCode()) {
			i++;
			index = (hashValue + i*i) % entries.length;
		}

		return index;
	}

	/**
	 * Helper method that resizes the entries array such that the number of occupied cells
	 * is less than half of the array's length.
	 */
	private void rehash() {
		//Figure out how large the array should be now
		int newSize = nextPrime(entries.length * 2);

		/** A reference to the old table */
		HashEntry[] temp = entries;

		/** The new, resized table that will contain the entries now. */
		entries = new HashEntry[newSize];

		occupiedCells = 0;

		for(int i = 0; i < temp.length; i++) {
			if(temp[i] != null && temp[i].active) {
				int index = findPosition(temp[i].element);
				entries[index] = new HashEntry(temp[i].element);
				occupiedCells++;
			}
		}
	}

	/**
	 * Insert a new item into the hash table
	 * @param item The object to be inserted
	 * @pre The item must not already exist within the table.
	 */
	public void insert(Object item) {
		/** Find the new item's location in the hash table */
		int index = findPosition(item);

		if(entries[index] == null) {
			//This element has never been in the hash table before!
			//Create a new entry for it and increment the cell-count
			HashEntry newEntry = new HashEntry(item);
			entries[index] = newEntry;
			occupiedCells++;

			//Rehash if the number of entries has grown too large.
			if(occupiedCells >= entries.length / 2) {
				rehash();
			}
		} else {
			//The entry for this item is either already entered, or is inactive.
			if(!entries[index].active) {
				//The entry must have been previously deleted. Re-activate it
				entries[index].active = true;
			}
		}
	}

	public void delete(Object item) {
		/** Find the item's location in the hash table */
		int index = findPosition(item);

		//Set the entry to inactive
		if(entries[index] != null && entries[index].active) {
			entries[index].active = false;
		}
	}

	/**
	 * Retrieve the specified object from the hash table.
	 * @param item The item to be found in the hash table
	 * @return The complete object from the hash table (with the same key as "item")
	 */
	public Object find(Object item) {
		/** Find the index of the requested item */
		int index = findPosition(item);

		//Return the item IF the element is non-null and active
		if(entries[index] != null && entries[index].active) {
			return entries[index].element;
		}

		//Default to null if no element found
		return null;
	}

	/**
	 * @return The number of active elements in the hash table
	 */
	public int elementCount() {
		int result = 0;
		
		for(int i = 0; i < entries.length; i++) {
			if(entries[i] != null && entries[i].active) {
				result++;
			}
		}
		
		return result;
	}

	/**
	 * @return True if the table is empty, false otherwise
	 */
	public boolean isEmpty() {
		for(int i = 0; i < entries.length; i++)  {
			if(entries[i] != null && entries[i].active) {
				return false;
			}
		}

		return true;
	}

	/** Wipes the hash table of all its values */
	public void makeEmpty() {
		entries = new HashEntry[entries.length];
	}

	/** Goes through the hash table and prints each key followed by its
	 * corresponding element. */
	public void printTable() {
		for(int i = 0; i < entries.length; i++) {
			System.out.print("[" + i + "]: ");
			if(entries[i] == null) {
				System.out.println("empty");
			} else {
				System.out.println(entries[i].element + ", " + 
						(entries[i].active ? "active" : "inactive"));
			}
		}
	}

	/**
	 * @return An Iterator object useful for iterating through the table
	 */
	public Iterator iterator() {
		return new Iter();
	}
}
