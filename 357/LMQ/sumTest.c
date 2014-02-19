#include <stdio.h>
#define MAX_INTS 100

int intList[MAX_INTS];
int sum;

int main(int argc, char *argv[]) {
	//printf("Enter the sum that each sequence should add to: ");
	scanf(" %d", &sum);
	
	//printf("Enter a bunch of integers: ");
	int integers = 0;
	while(scanf(" %d",&intList[integers])) {
		if(intList[integers++] == 0) break;
	}
	integers --;
	
	int i, cursor;
	for(i = 0; i < integers; i ++) {
		int currentSum = 0;
		for(cursor = i; ;) {
			currentSum += intList[cursor];
			if(currentSum == sum)
				printf("From %d to %d\n",i,cursor);

			if(cursor >= integers - 1) cursor = -1;
			if(++cursor == i) break;
		}
	}	
	return 0;
}
