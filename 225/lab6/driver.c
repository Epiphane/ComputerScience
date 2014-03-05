#include "stack.h"
#include <stdio.h>

int main(int argc, char* argv) {
	printf("Welcome to the stack program.");

	char input;
	int outputFormat = 0;
	while(input != 'x') {
		// Get user input
		printf("\n\nEnter option: ");
		scanf(" %c", &input);

		// Perform the action
		int usefulNumber;
		switch(input) {
		case 'u':			// Push a number
			printf("What number? ");
			scanf("%d", &usefulNumber);

			if(push(usefulNumber) == 1)
				printf("Overflow!!!\n");
			
			break;
		case 'o':			// Pop a number
			if(pop(&usefulNumber) == 1)
				printf("Underflow!!!\n");
			else
				printf("Popped %d\n", usefulNumber);

			break;
		case 'd':			// Print in decimals
			outputFormat = 0;
			break;
		case 'h':			// Print in hex
			outputFormat = 1;
			break;
		case 'c':			// Print in chars
			outputFormat = 2;
			break;
		case 'x':
			printf("Goodbye!\n");
		}
		if(input != 'x') {
			printStack(outputFormat);
		}
	}

	return 0;
}
