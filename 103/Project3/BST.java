import java.util.Iterator;
import java.util.NoSuchElementException;

/****
 * 
 * Class BST implements a Binary Search Tree data structure with a linked list.
 * It provides methods to add, delete, find, and iterate through elements.
 * 
 * @author Thomas Steinke, Elliot Fiske
 *
 * @param <T> The type of element
 */

public class BST <T extends Comparable<? super T>>
{
	/** The root of the entire tree */
	BSTNode root;
	
	/** Class BSTNode contains an element and a pointer to its left child and right child.*/
	private class BSTNode {
		/** The element contained by this node. */
		public T element;
		/** The left and right child of the current node. */
		public BSTNode left, right;
	}
	
	/** Class PreIter provides methods to traverse the tree in pre-order. */
	private class PreIter implements Iterator<T> {
		private MyStack<T> stack;
		
		public PreIter() {
			stack = new MyStack<T>();
			addNode(root);
		}
		
		private void addNode(BSTNode node) {
			if(node == null) return;
			
			addNode(node.right);
			addNode(node.left);
			stack.push(node.element);
		}
		
		/** Returns true if there is an element not yet covered by the iterator,
		 *  false otherwise. */
		@Override
		public boolean hasNext() {
			return !stack.isEmpty();
		}

		
		@Override
		public T next() {
			if(stack.isEmpty()) throw new NoSuchElementException();
			return stack.pop();
		}

		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}
		
	}


	/** Class InIter provides methods to traverse the tree in in-order. */
	private class InIter implements Iterator<T> {
		private MyStack<T> stack;
		
		public InIter() {
			stack = new MyStack<T>();
			addNode(root);
		}
		
		private void addNode(BSTNode node) {
			if(node == null) return;
			
			addNode(node.right);
			stack.push(node.element);
			addNode(node.left);
		}
		
		/** Returns true if there is an element not yet covered by the iterator,
		 *  false otherwise. */
		@Override
		public boolean hasNext() {
			return !stack.isEmpty();
		}

		@Override
		public T next() {
			if(stack.isEmpty()) throw new NoSuchElementException();
			return stack.pop();
		}

		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}
		
	}

	/** Class LevelIter provides methods to traverse the tree in levelorder. */
	private class LevelIter implements Iterator<T> {
		private LQueue<BSTNode> queue;
		
		public LevelIter() {
			queue = new LQueue<BSTNode>();
			if(root != null)
				queue.enqueue(root);
		}
		
		/** Returns true if there is an element not yet covered by the iterator,
		 *  false otherwise. */
		@Override
		public boolean hasNext() {
			return !queue.isEmpty();
		}

		@Override
		public T next() {
			if(queue.isEmpty()) throw new NoSuchElementException();

			BSTNode thisNode = queue.dequeue();
			if(thisNode.left != null) queue.enqueue(thisNode.left);
			if(thisNode.right != null) queue.enqueue(thisNode.right);
			
			return thisNode.element;
		}

		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}
		
	}
	
	public static class MyException extends RuntimeException {
		
	}
	
	/** 
	 * The insert method puts a new element of type T into the binary search tree.
	 * @param element The new element to be added
	 * @pre element is not null
	 */
	public void insert(T element) {
		if(root == null) {
			root = new BSTNode();
			root.element = element;
		} else {
			insertUnder(element, root);
		}
	}

	/**
	 * Recursive helper method for the insert method.
	 * @param element The current element that is being added
	 * @param node The next node to check against.
	 */
	private void insertUnder(T element, BSTNode node) {
		//If the element is LESS than the current node we're examining,
		//go to the LEFT side of the tree.
		if(element.compareTo(node.element) < 0) {
			if(node.left == null) {
				node.left = new BSTNode();
				node.left.element = element;
			} else {
				insertUnder(element, node.left);
			}
		//If the element is GREATER than the current node we're examining,
		//go to the RIGHT side of the tree.
		} else {
			if(node.right == null) {
				node.right = new BSTNode();
				node.right.element = element;
			} else {
				insertUnder(element, node.right);
			}
		}
	}
	
	/** 
	 * Searches the tree for the specified item then deletes that item.
	 * @param item The element to delete from the tree.
	 */
	public void delete(T item) {
		delete(item, root);
	}
	
	/**
	 * Checks to see if node is the item we're trying to delete.
	 * 
	 * @param item
	 * @param node
	 * @param wentLeft
	 * @return 1 if we deleted an item, 0 if no item found in tree, -1 if we need to delete this node
	 */
	private BSTNode delete(T item, BSTNode node) {
		if(node == null) return node;
		
		int compareResult = item.compareTo(node.element);
		if(compareResult < 0) { // Move left
			node.left = delete(item, node.left);
		}
		else if(compareResult > 0) { // Move right
			node.right = delete(item, node.right);
		}
		else if(node.left != null && node.right != null) { // This is the item!! and there are 2 children
			node.element = findMinimum(node.right);
			node.right = delete(node.element, node.right);
		}
		else { // Not 2 children
			node = (node.left != null) ? node.left : node.right;
		}
		
		return node;
	}
	
	/**
	 * Looks through the tree to find an item.
	 * @return true if found, otherwise false
	 */
	public boolean find(T item) {
		return find(item, root);
	}
	
	/**
	 * Looks under a specific node for an item
	 * 
	 * @param item
	 * @param node
	 * @return
	 */
	private boolean find(T item, BSTNode node) {
		if(node == null) return false;
		
		if(node.element.equals(item))
			return true;
		
		return find(item, node.left) || find(item, node.right);
	}
	
	/**
	 * @return True if the tree contains no elements, false if it does contain elements.
	 */
	public boolean isEmpty() {
		return root == null;
	}
	
	/** Reinitalizes the binary tree by setting the current root to null. */
	public void makeEmpty() {
		root = null;
	}
	
	/** Returns the current number of elements stored in the tree. */
	public int size() {
		return size(root);
	}
	
	/** A private recursive helper method to calculate the size of the tree. */
	private int size(BSTNode node) {
		if(node == null) return 0;
		return 1 + size(node.left) + size(node.right);
	}
	
	/** Returns the smallest value in the tree */
	public T findMinimum() {
		if(root == null) throw new MyException();
		
		return findMinimum(root);
	}
	
	/**
	 * Find minimum value under a certain node
	 * 
	 * @param node
	 * @return
	 */
	private T findMinimum(BSTNode node) {
		if(node.left == null) return node.element;
		else return findMinimum(node.left);
	}
	
	/** Returns the largest value in the tree */
	public T findMaximum() {
		if(root == null) throw new MyException();
		
		return findMaximum(root);
	}
	
	/**
	 * Find maximum value under a certain node
	 * 
	 * @param node
	 * @return
	 */
	private T findMaximum(BSTNode node) {
		if(node.right == null) return node.element;
		else return findMaximum(node.right);
	}
	
	/**
	 * Return the three different kinds of iterators: PreIter, InIter, and LevelIter
	 */
	public Iterator<T> iteratorPre() {
		return new PreIter();
	}
	
	public Iterator<T> iteratorIn() {
		return new InIter();
	}
	
	public Iterator<T> iteratorLevel() {
		return new LevelIter();
	}
	
	/**
	 * Print the contents of the tree
	 */
	public void printTree() {
		printNode(root, "");
	}
	
	/**
	 * Print a specific node
	 */
	public void printNode(BSTNode node, String tabbation) {
		if(node == null) return;
		
		System.out.println(tabbation + node.element);
		printNode(node.left, tabbation+"\t");
		printNode(node.right, tabbation+"\t");
	}
	
	/**
	 * Returns the tree in pre-ordered string format
	 */
	@Override
	public String toString() {
		return nodeString(root);
	}
	
	/**
	 * Returns the element in a string, Pre-order
	 */
	public String nodeString(BSTNode node) {
		if(node == null) return "";
		
		return node.element + " " + nodeString(node.left) + nodeString(node.right);
	}
}
