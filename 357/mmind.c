#include <stdio.h>
#include <ctype.h>
 
#define MAXDIM 10
#define MAXCHAR 'F'

static int seed;

// This will return a different random nonnegative integer on
// each successive call.

int rand() {
   seed = (seed * 131071 + 524287) % 8191;
   return seed;
}

// Call this just once in the entire program, with the seed.
void srand(int s) {
   seed = s;
}

void initialize(char model[], int dimensions, char maxchar);
int match(char model[], char guess[], int dimensions);
int non_blank();
int get_guess(char guess[], int dimensions, char maxchar, int try);

/* Initialize "model" to contain a random pattern of "dimensions"
 * letters, in the range 'A' to "maxchar".
 */
void initialize(char model[], int dimensions, char maxchar)
{
   int i;
   for(i = 0; i < dimensions; i ++) {
      model[i] = 
   }
}
 
/* Compare "model" to "guess", assuming each contains "dimensions"
 * characters.  Print out the number of exact and inexact matches.
 * Returns the number of exact matches. 
 */
int match(char model[], char guess[], int dimensions)
{
}
 
/* Return the next non_blank character from the input.  Return EOF if
 * you hit EOF while trying to find the next nonblank character
 */
int non_blank(void)
{
   char result;
   if(EOF == scanf(" %c",&result)) {
      return EOF;
   }
   return result;
}
 
/* Input the user's guess, putting it into "guess".  Assume that
 * there will be "dimensions" characters in the guess, in the range
 * 'A' to "maxchar".  Assume that this is the "try"th attempt on the
 * user's part.  If a guess with the wrong format or characters is
 * entered, make the user try again until a valid guess is entered.
 * After each guess, read and discard any extra characters up to and
 * including the end of line.  Return true if a guess is obtained, 
 * false if EOF is hit while trying to get a good guess.
 */
int get_guess(char guess[], int dimensions, char maxchar, int try)
{
   int i;
   // Collect all guesses, if one doesn't work return false
   for(i = 0; i < dimensions; i ++) {
      if(EOF == (guess[i] = non_blank(void)))
         return false;
   }
   return true;
}
 
int main()
{
   // Set up variables
   char maxchar;
   int dimensions, seed, numTries, numGames = 0;
   double average = 0;
   /*
    * flag keeps track of what state we're in:
    * -1: Running normally
    * 0: User won the game
    * 1: EOF error
    */
   int flag = -1;

   // Run for as long as the game is set to run normally
   while(flag == -1) {
      // Input initial values
      printf("Enter maxchar, dimensions, and seed => ");
      if(EOF != scanf("%c %d %d", &maxchar, &dimensions, &seed) {
         // EOF Error
         flag = 1;
      }
      else {
         // Check for validity of input
         if(maxchar > MAXCHAR || dimensions > MAXDIM) {
            printf("Bad initial values");
            return 1;
         }
      
         // Create the model and guess arrays and set the amount of tries so far to 0
         char model[dimensions], guess[dimensions];
         numTries = 0;
    
         // Initialize the model
         initialize(model, dimensions, maxchar);
     
         // Keep guessing until the model and guess match each other
         while(flag < 0) {
            if(get_guess(guess, dimensions, maxchar, numTries)) {
               // Check against current model,
               // Set flag to negative however many we're missing
               flag = match(model, guess, dimensions) - dimensions;
            }
            else {
               // EOF error!
               flag = 1;
            }
         }
         
         // Check for a new game as long as we don't have an EOF error
         if(flag != 1) {
            // Calculate new average
            average *= numGames;
            average += numTries;
            average /= ++numGames;

            // Display average and ask for new game
            printf("Current Average: &.3f\nAnother game [Y/N]?", average);
            char response;
            scanf("%c", &response);
            if(response == 'Y' || response == 'y') {
               // User didn't to end
               flag = -1;
            }
         }
      }
   }

   if(flag == 1) {
      printf("Unexpected EOF\n");
   }

   return 0;
}
