public class Student extends Person {
   private double gpa;

   public Student(double gpa, String name, int legs) {
      super(name, legs);
      this.gpa = gpa;
   }

   public boolean equals(Object other) {
      if(!super.equals(other)) {
         return false;
      }      
      if(gpa != ((Student)other).gpa) {
         return false;
      }
      return true;
   }

   public String toString() {
      return super.toString() + " and a Student Object with a "+gpa+" gpa";
   }
}
