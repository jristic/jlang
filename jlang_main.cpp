#include "jlang_file.h"
#include "jlang_parse.h"

#include <stdio.h>
#include <stdlib.h>

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
	assert(argc == 2, "wrong number of args");

	char* filename = argv[1];

	const uint MAX_INPUT_FILE_SIZE = 1024 * 1024;
	char input_buffer[MAX_INPUT_FILE_SIZE];
	int input_len = 0;

	File::ReadFile(
		filename,
		input_buffer,
		&input_len,
		MAX_INPUT_FILE_SIZE);

	Parse::ASTNode* program_ast = Parse::ParseBuffer(input_buffer, input_len);
}

#include "jlang_file.cpp"
#include "jlang_parse.cpp"