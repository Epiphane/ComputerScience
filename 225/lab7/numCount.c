#include "numCount.h"
#include <stdio.h>

int main(int argc, char* argv) {
	printf("Enter a number: ");

	int numberOfMagicalDelusion;
	scanf("%d",&numberOfMagicalDelusion);

	countBackwardFrom(numberOfMagicalDelusion);
	printf("\n");
	countForwardTo(numberOfMagicalDelusion);
	printf("\n");

	return 0;
}

void countBackwardFrom(int x) {
	// Print me!
	printf("%d ",x);

	// Continue if x > 1
	if(x > 1) countBackwardFrom(x-1);
}

void countForwardTo(int x) {
	// Continue moving down if x > 1
	if(x > 1) countForwardTo(x-1);

	// Print me!
	printf("%d ",x);
}
