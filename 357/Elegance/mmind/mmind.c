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
   int i, range = maxchar - '@';
   for(i = 0; i < dimensions; i ++) {
      model[i] = rand() % range + 'A';
   }
}

/* Compare "model" to "guess", assuming each contains "dimensions"
 * characters.  Print out the number of exact and inexact matches.
 * Returns the number of exact matches.
 */
int match(char model[], char guess[], int dimensions)
{
   int exact = 0, inexact = 0;
   int i, j;
   int results[dimensions];
   // Identify all exact matches
   for(i = 0; i < dimensions; i ++) {
      exact += results[i] = model[i] == guess[i];
   }

   for(i = 0; i < dimensions; i ++) {
      // Dont check for inexact guesses if it has an exact match
      for(j = 0; j < dimensions; j ++) {
         // Continue if it would be an exact match
         // or if it already has a match
         if(results[i] != 1 && !results[j] && model[j] == guess[i]) {
            inexact ++;
            results[j] += 2;
         }
      }
   }

   printf("    %d Exact and %d Inexact\n",exact,inexact);
   return exact;
}

/* Return the next non_blank character from the input.  Return EOF if
 * you hit EOF while trying to find the next nonblank character
 */
int non_blank(void)
{
   char result = EOF;
   scanf(" %c",&result);

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
   printf("\n %d. Enter your guess: ",try);
   int i;
   // Collect all guesses, if one doesn't work return false
   for(i = 0; i < dimensions; i ++) {
      if(EOF == (guess[i] = non_blank())) {
         return 0;
      }

      // Try to compensate for lowercase characters
      if(guess[i] >= 'a') guess[i] -= 32;

      // Still outta bounds?
      if(guess[i] < 'A' || guess[i] > maxchar) {
         printf("    Bad entry.  Try again: ");
         while(getchar() != '\n');
         i = -1;
      }
   }
   while(getchar() != '\n');
   return 1;
}

int main()
{
   // Set up variables
   char maxchar, response;
   int dimensions, seed, numGames = 0, numTries;
   double totalTries = 0;
   char model[dimensions], guess[dimensions];

   /*
    * flag keeps track of what state we're in:
    * 1: Running normally
    * 0: User won the game
    * -1: EOF error
    */
   int flag = 1;

   // Input initial values
   printf("Enter maxchar, dimensions, and seed => ");
   scanf(" %c %d %d", &maxchar, &dimensions, &seed);
   
   // Set up seed
   srand(seed);

   // Check for validity of input
   if(maxchar > MAXCHAR) maxchar -= ' ';
   if(maxchar > MAXCHAR || maxchar < '@' || dimensions > MAXDIM || dimensions <= 0) {
      printf("Bad initial values\n");
      flag = 0;
   }

   // Run for as long as the game is set to run normally
   while(flag == 1) {
      flag ++;
      // Initialize the model
      initialize(model, dimensions, maxchar);
      numTries = 0;

      // Keep guessing until the model and guess match each other
      while(get_guess(guess, dimensions, maxchar, ++numTries) &&
            (flag = match(model,guess,dimensions) - dimensions));

      // Check for a new game as long as we don't have an EOF error
      if(flag == 0) {
         totalTries += numTries;

         // Display average and ask for new game
         printf("\n\nCurrent average:  %.3f\n\nAnother game [Y/N]? ", totalTries / ++numGames);
         flag = -(EOF == scanf(" %c", &response));
         if(response == 'Y' || response == 'y') {
            // User didn't want to end, set flag back to 0
            flag++;
         }
      }
   }
   

   if(flag != 0)
      printf("Unexpected EOF\n");

   return 0;
}

