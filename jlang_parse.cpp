
#include <cctype>

namespace Parse
{

const int MAX_AST_NODES = 1024;
enum Token
{
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_IDENTIFIER,
	TOKEN_INTEGER_LITERAL
};

struct Identifier {
	const char* string;
	int string_len;
};

ASTNode ASTNodePool[MAX_AST_NODES];
int num_allocated_ast_nodes = 0;

ASTNode* AllocASTNode()
{
	int ret_index = num_allocated_ast_nodes;
	++num_allocated_ast_nodes;
	assert(num_allocated_ast_nodes < MAX_AST_NODES, "out of ast nodes");
	return &ASTNodePool[ret_index];
}

int SkipWhitespace(
	char* buffer,
	int buffer_start,
	int buffer_len)
{
	int buffer_next = buffer_start;
	while (buffer_next < buffer_len &&
		isspace(buffer[buffer_next]))
	{
		++buffer_next;
	}
	return buffer_next;
}

Token GetNextToken(
	char* buffer,
	int buffer_start,
	int buffer_len,
	int* out_characters_read)
{
	int buffer_next = buffer_start;
	Token retval;

	if (buffer[buffer_next] == '(') {
		++buffer_next;
		retval = TOKEN_LPAREN;
	}
	else if (buffer[buffer_next] == ')') {
		++buffer_next;
		retval = TOKEN_RPAREN;
	}
	else if (isdigit(buffer[buffer_next])) {
		++buffer_next;
		while(buffer_next < buffer_len &&
			isdigit(buffer[buffer_next])) 
		{
			++buffer_next;
		}
		retval = TOKEN_INTEGER_LITERAL;
	}
	else if (islower(buffer[buffer_next]) 
		|| isupper(buffer[buffer_next]))
	{
		++buffer_next;
		while (buffer_next < buffer_len &&
			(islower(buffer[buffer_next]) || 
			isupper(buffer[buffer_next]))) 
		{
			++buffer_next;
		}
		retval = TOKEN_IDENTIFIER;
	}
	else {
		assert(false, "unexpected character when reading token: %c",
			buffer[buffer_next]);
	}

	*out_characters_read = buffer_next - buffer_start;
	return retval;
}

Identifier GetIdentifier(
	char* buffer,
	int buffer_start,
	int buffer_len,
	int* out_characters_read)
{
	int buffer_next = buffer_start;
	while (buffer_next < buffer_len &&
		(islower(buffer[buffer_next]) || 
		isupper(buffer[buffer_next]))) 
	{
		++buffer_next;
	}
	Identifier id;
	id.string = buffer + buffer_start;
	id.string_len = buffer_next - buffer_start;
	return id;
}

ASTNode* ParseBuffer(
	char* buffer,
	int buffer_start,
	int buffer_len,
	int* out_bytes_read)
{
	int buffer_next = buffer_start;
	while (buffer_next < buffer_len) {
		buffer_next = SkipWhitespace(buffer, buffer_next, buffer_len);
		int tok_size;
		Token tok = GetNextToken(buffer, buffer_next, buffer_len, &tok_size);
		buffer_next += tok_size;
		printf("%d %d \n", tok, tok_size);
		buffer_next = SkipWhitespace(buffer, buffer_next, buffer_len);
	}
	*out_bytes_read = buffer_next - buffer_start;
	return null;
}

ASTNode* ParseBuffer(
	char* buffer,
	int buffer_len)
{
	int bytes_read;
	int buffer_start = 0;
	ASTNode* ast = ParseBuffer(
		buffer,
		buffer_start,
		buffer_len,
		&bytes_read);
	assert(bytes_read == buffer_len, "not all of the buffer was consumed");
	return ast;
}

}