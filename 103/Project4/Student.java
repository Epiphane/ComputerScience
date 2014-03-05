/****
 * Class Student represents a student of a school system.  It provides storage
 * for their id number and last name
 * @author Elliot Fiske, Thomas Steinke, efiske, tsteinke
 *
 */

public class Student {
	/** A unique student identification number */
	private long id;
	/** The student's last name (need not be unique) */
	private String lastName;

	/**
	 * Constructs a new Student with the given id and name
	 * @param id The student's unique ID number
	 * @param lastName The student's last name
	 */
	public Student(long id, String lastName) {
		this.id = id;
		this.lastName = lastName;
	}
	
	/**
	 * @return True if the other Student is equal to this one, false otherwise
	 */
	public boolean equals(Object other) {
		//Null student's can't be equal to non-null students
		if(other == null) return false;
		
		//Make sure the other object is a Student
		if(other instanceof Student) {
			//Check if they've got the same ID
			return ((Student) other).id == id;
		} else {
			return false;
		}
	}
	
	/**
	 * Returns the string representation of this Student
	 * @return A string in the format {id: ###, name: NAME}
	 */
	public String toString() {
		return "{ id: " + id + ", name: " + lastName + "}";
	}
	
	/**
	 * Hashcode returns a unique hash code usable by a hash table
	 * @return A unique, identifying hash based on the student ID
	 */
	public int hashCode() {
		return Long.valueOf(id).hashCode();
	}
}
