import java.util.EmptyStackException;

/****
 * MyStack is a linked-list based implementation of a stack ADT.
 * 
 * @author Thomas Steinke, Elliot Fiske
 *
 * @param <T> The type of data that will be stored in the stack
 */

public class MyStack<T> {
	/** The most recent element to be pushed, and the next to be popped. */
	private Node first;
	
	private class Node {
		/** The data being held by this Node. */
		public T element;
		/** The Node that follows this one in the stack. */
		public Node next;
	}
	
	public MyStack() {
		first = null;
	}
	
	public void push(T element) {
		//Initialize a node and fill it with the specified element, then connect
		//it to the stack and mark it as the "first" Node.
		Node node = new Node();
		node.element = element;
		node.next = first;
		
		first = node;
	}
	
	public T pop() {
		//If the list hasn't been intialized, "first" will be null. Throw an exception.
		if(first == null) {
			throw new EmptyStackException();
		}
		Node temp = first;
		first = first.next;
		
		return temp.element;
	}
	
	public T peek() {
		//If the list hasn't been intialized, "first" will be null. Throw an exception.
		if(first == null) {
			throw new EmptyStackException();
		}
		return first.element;
	}
	
	public boolean isEmpty() {
		return first == null;
	}
}
