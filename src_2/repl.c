#include <stdio.h>
#include <stdlib.h>

/* Declare a buffer for user input of size 2048 */
#define INPUT_SIZE 2048
static char input[INPUT_SIZE];

int main(int argc, char* argv[argc+1]) {
	printf("Lispy Version 0.0.0.0.1\n");
	printf("Press Ctrl-C to Exit\n");

	while (1) {
		// Output our prompt
		fputs("lispy> ", stdout);

		// Read a line of user input of max size 2048 bytes
		fgets(input, INPUT_SIZE , stdin);

		printf("No you're a %s", input);
	}

	return 0;
}
