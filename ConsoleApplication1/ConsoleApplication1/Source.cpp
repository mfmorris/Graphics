#include <stdio.h>

int decide(int a, int b, int c) {
	int* result = new int();
	int* result2 = result + a;
	*result2 = b;
	*result = c;
	return *(result2);
}

int main() {
	printf("%d\n", decide(0, 1, 2));
	printf("%d", decide(1, 1, 2));
	while (1){}
}