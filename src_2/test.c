#include <stdio.h>
#include "mpc.h"

int main() {

	char* input = "aabaab";
	mpc_parser_t* String = mpc_new("string");	

	mpca_lang(MPCA_LANG_DEFAULT,
			"			\
				input	: /^[ab]+[ab]*$/ ;	\
			", String);

	mpc_result_t r;
	if(mpc_parse("<stdin>", input, String, &r)) {
		mpc_ast_print(r.output);
		mpc_ast_delete(r.output);
	} else {
		mpc_err_print(r.error);
		mpc_err_delete(r.error);
	}
}
