#include "jlang_file.h"
#include "jlang_parse.h"
#include "jlang_interp.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#define null 0
#define assert(expression, message, ...) 				\
	do { 												\
		if (!(expression)) {							\
			fprintf(stderr, "/* ---- ASSERT ---- */\n");\
			fprintf(stderr,								\
				"LOCATION:   %s@%d\nCONDITION:  %s \n",	\
				__FILE__, __LINE__, #expression); 		\
			fprintf(stderr, "MESSAGE:    ");			\
			fprintf(stderr, message, ##__VA_ARGS__);	\
			fprintf(stderr, "\n");						\
			__builtin_trap();							\
		}												\
	} while (0);										\

const char* jlang_usage = "USAGE: jlang [-i] [-o output_file] input_file";

typedef unsigned int uint;

int main(int argc, char* argv[]) {
	assert(argc >= 2 && argc <= 3, "wrong number of args \n%s", jlang_usage);

	bool interp = false;
	const char* filename = "";

	for (int i = 1 ; i < argc ; ++i) {
		char* param = argv[i];
		if (param[0] == '-') {
			assert(param[1] = 'i' && strlen(param) == 2,
				"unknown command option: %s", param);
			interp = true;
		}
		else {
			filename = param;
		}
	}

	const uint MAX_INPUT_FILE_SIZE = 1024 * 1024;
	char input_buffer[MAX_INPUT_FILE_SIZE];
	int input_len = 0;

	File::ReadFile(
		filename,
		input_buffer,
		&input_len,
		MAX_INPUT_FILE_SIZE);
	printf("INPUT PROGRAM: \n%s \n", input_buffer);

	Parse::ASTNode* program_ast = Parse::ParseBuffer(input_buffer, input_len);

	if (interp) {
		Interp::InterpResult interp_result = Interp::InterpAST(program_ast);
		printf("INTERP RESULT:\n%d\n", interp_result.intResult);
	}
}

#include "jlang_file.cpp"
#include "jlang_parse.cpp"
#include "jlang_interp.cpp"