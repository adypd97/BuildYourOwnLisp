#include <stdio.h>
#include <stdlib.h>

/* If we are compiling on a Windows machine */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char* cpy = malloc(strlen(buffer)+1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy)+1] = '\0';
	return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

/* Otherwise include the editline headers */
#else 

// on MAC this will allow us to edit our input 
// to the REPL, so that weird characters don't appear
// when we press arrows keys 
#include <editline/readline.h>
#endif

/* Declare a buffer for user input of size 2048 */
//#define INPUT_SIZE 2048
//static char input[INPUT_SIZE];

int main(int argc, char* argv[argc+1]) {
	// Print Version and Exit Information
	printf("Lispy Version 0.0.0.0.1\n");
	printf("Press Ctrl-C to Exit\n");

	while (1) {
		// Output our prompt
		char *input = readline("lispy> ");

		// add input to history
		add_history(input);

		printf("You entered: %s\n", input);

		// free retrieved input
		free(input);
	}

	return 0;
}
