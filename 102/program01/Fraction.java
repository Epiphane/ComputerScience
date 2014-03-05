/**
  * Fraction class
  * This class holds a numerator and denominator.
  * and allows for basic fraction math, including
  * multiplication, division, addition, and subtraction.
  *
  * @author Thomas Steinke
  * @version Program 1
  */
  
public class Fraction {
   private int num, denom;
   
   public Fraction() {
      denom = 1;
   }
   
   public Fraction(int numerator) {
      num = numerator;
      denom = 1;
   }

   public Fraction(int numerator, int denominator) {
      if(denominator <= 0) {
         throw new IllegalArgumentException();
      }
      
      num = numerator;
      denom = denominator;
      
      simplify();
      if(denom < 0) {
    	  num *= -1;
    	  denom *= -1;
      }
   }
   
   public Fraction add(Fraction other) {
      int nDenom = denom * other.getDenominator();
      int nNum = (num * other.getDenominator()) + (other.getNumerator() * denom);
      
      return new Fraction(nNum, nDenom);
   }
   
   public Fraction sub(Fraction other) {
      int nDenom = denom * other.getDenominator();
      int nNum = (num * other.getDenominator()) - (other.getNumerator() * denom);
      
      return new Fraction(nNum, nDenom);
   }
   
   public Fraction mul(Fraction other) {
      int nDenom = denom * other.getDenominator();
      int nNum = num * other.getNumerator();
      
      return new Fraction(nNum, nDenom);
   }
   
   public Fraction div(Fraction other) {
      int nDenom = denom * other.getNumerator();
      int nNum = num * other.getDenominator();
      if(nDenom <= 0) {
    	  nNum *= -1;
    	  nDenom *= -1;
      }
      
      return new Fraction(nNum, nDenom);
   }
   
   public boolean equals(Fraction other) {
      if(other.getNumerator() != num || other.getDenominator() != denom) {
         return false;
      }
      return true;
   }
   
   public int getDenominator() {
      return denom;
   }
   
   public int getNumerator() {
      return num;
   }
   
   public String toString() {
      if(denom == 1) {
         return num+"";
      }
      return num+"/"+denom;
   }
   
   public double value() {
      if(denom == 0) {
         System.out.println("Dude. You can't divide by 0. Way to go.");
         return 0;
      }
      return (double) num/denom;
   }
   
   private void simplify() {
      // Get the GCD of the numerator and denominator
      int gcd = GCD(num, denom);
      
      num /= gcd;
      denom /= gcd;
   }

   // Euclid's GCD Algorithm
   // Process found at http://en.wikipedia.org/wiki/Euclidean_algorithm
   private int GCD(int n1, int n2) {
      while (n2 != 0) {
         int temp = n2;
         n2 = n1 % n2;
         n1 = temp;
      }
      return n1;
   }
}