#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

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
//#include <editline/history.h>
#endif

/* Declare a buffer for user input of size 2048 */
//#define INPUT_SIZE 2048
//static char input[INPUT_SIZE];

// Use operator string to see which operation to perform
long eval_op(long x, char* op, long y) {
	if (strcmp(op, "+") == 0) { return x + y; }
	if (strcmp(op, "add") == 0) { return x + y; }
	if (strcmp(op, "-") == 0) { return x - y; }
	if (strcmp(op, "sub") == 0) { return x - y; }
	if (strcmp(op, "*") == 0) { return x * y; }
	if (strcmp(op, "mul") == 0) { return x * y; }
	if (strcmp(op, "/") == 0) { return x / y; }
	if (strcmp(op, "div") == 0) { return x / y; }
	if (strcmp(op, "%") == 0) { return x % y; }
	return 0;
}

// Traverse the AST to compute the value of the expression
// input at the REPL.
long eval(mpc_ast_t* t) {
	// base case: if tag is a number
	// then just return its content as long
	if(strstr(t->tag, "number")) {
		return atoi(t->contents);
	}

	// the operator in a 'expr' is always the 
	// second child. since first is '('
	char* op = t->children[1]->contents;

	long x = eval(t->children[2]);

	int i = 3;
	while(strstr(t->children[i]->tag, "expr")) {
		x = eval_op(x, op, eval(t->children[i]));
		i++;
	}

	return x;
}

int main(int argc, char* argv[argc+1]) {
	// Create some parsers, explained in Polish Notation Section in notes
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Text_Operator = mpc_new("text_operator");
	mpc_parser_t* Sym_Operator = mpc_new("sym_operator");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Lispy = mpc_new("lispy");
	
	// Define them with the following language
	mpca_lang(MPCA_LANG_DEFAULT,
		"				\
			number	: /-?[0-9]+([.][0-9]+)?/ ;	\
			sym_operator: '+' | '-' | '*' | '/' | '%' ;	\
			text_operator: \"add\" | \"sub\" | \"mul\" | \"div\" ;	\
			operator: <sym_operator> | <text_operator> ;	\
			expr	: <number> | '(' <operator> <expr>+ ')' ;	\
			lispy	: /^/ <operator> <expr>+ /$/ ;	\
		",
		Number, Operator, Text_Operator, Sym_Operator, Expr, Lispy);



	// Print Version and Exit Information
	printf("Lispy Version 0.0.0.0.1\n");
	printf("Press Ctrl-C to Exit\n");

	while (1) {
		// Output our prompt
		char *input = readline("lispy> ");

		// add input to history
		add_history(input);

		// Parse the input from the user
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Lispy, &r)) {
			/* On success print the AST */
			//mpc_ast_print(r.output);
			long result = eval(r.output);
			printf("%li\n", result);
			mpc_ast_delete(r.output);
		} else {
			/* Otherwise print error */
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}

		// free retrieved input
		free(input);
	}
	
	// Undefine and Delete the parsers.
	mpc_cleanup(4, Number, Operator, Expr, Lispy);
	return 0;
}
