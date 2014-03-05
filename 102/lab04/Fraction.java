public class Fraction {
   private int num, denom;
   
   public Fraction() {
      denom = 1;
   }
   
   public Fraction(int numerator) {
      denom = 1;
   }

   public Fraction(int numerator, int denominator) {
      denom = 1;
   }
   
   public Fraction add(Fraction other) {
      return new Fraction();
   }
   
   public Fraction sub(Fraction other) {
      return new Fraction();
   }
   
   public Fraction mul(Fraction other) {
      return new Fraction();
   }
   
   public Fraction div(Fraction other) {
      return new Fraction();
   }
   
   public boolean equals(Fraction other) {
      return false;
   }
   
   public int getDenominator() {
      return denom;
   }
   
   public int getNumerator() {
      return num;
   }
   
   public String toString() {
      return num+"/"+denom;
   }
   
   public double value() {
      return num/denom;
   }
}