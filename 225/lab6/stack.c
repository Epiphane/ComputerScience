 #include <stdio.h>

int stack[10]; 
int topOfStack = -1;

int push(int value) {
	if(topOfStack >= 9) {
		return 1;
	}
	stack[++topOfStack] = value;
	return 0;
}

int pop(int *value) {
	if(topOfStack < 0) {
		return 1;
	}
	*value = stack[topOfStack--];
	return 0;
}

void printStack(int format) {
	printf("Stack: ");

	int i = 0;
	switch(format) {
	case 0: // Decimal
		for(i = 0; i <= topOfStack; i ++) {
			printf("%d ",stack[i]);
		}
		break;
	case 1: // Hex
		for(i = 0; i <= topOfStack; i ++) {
			printf("%x ",stack[i]);
		}
		break;
	case 2: // Chracters
		for(i = 0; i <= topOfStack; i ++) {
			printf("%c ",stack[i]);
		}
		break;
	default:
		printf("\nWe don't use that format, silly");
		break;
	}
}
