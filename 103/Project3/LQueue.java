/****
 * 
 * Class LQueue is a queue using FIFO storage. Used in the InIter class of BST
 * 
 * @author Thomas Steinke, Elliot Fiske
 *
 * @param <T> The type of element
 */
public class LQueue <T> {
	
	// Class Node stores each element
	private class Node {
		public Node(T element) {
			this.element = element; 
		}
		public T element;
		public Node next;
	}
	
	public static class MyException extends RuntimeException {
	
	}
	
	private Node front;
	private Node end;
	
	public LQueue() {
		end = front = null;
	}
	
	public void enqueue(T element) {
		Node newNode = new Node(element);
		
		//Initialize empty list
		if(front == null) {
			front = end = newNode;
		} else {
			end.next = newNode;
			end = newNode;
		}
	}
	
	public T dequeue() {
		if(front == null) {
			throw new MyException();
		}
		
		T temp = front.element;
		front = front.next;
		return temp;
	}
	
	public boolean isEmpty() {
		return front == null;
	}
}
