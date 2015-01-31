
#include <cctype>
#include <math.h>
#include <cstring>

namespace Parse
{

enum Token {
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_IDENTIFIER,
	TOKEN_INTEGER_LITERAL
};

struct Identifier {
	const char* string;
	int string_len;
};

const int MAX_AST_NODES = 1024;
ASTNode ASTNodePool[MAX_AST_NODES];
int num_allocated_ast_nodes = 0;

ASTNode* AllocASTNode() {
	int ret_index = num_allocated_ast_nodes;
	++num_allocated_ast_nodes;
	assert(num_allocated_ast_nodes < MAX_AST_NODES, "out of ast nodes");
	return &ASTNodePool[ret_index];
}

bool CompareIdentifier(Identifier id, const char* string) {
	if (id.string_len != strlen(string))
		return false;
	for (int i = 0 ; i < id.string_len ; ++i)
		if (id.string[i] != string[i])
			return false;
	return true;
}

char* SkipWhitespace(
	char* buffer_start,
	char* buffer_end)
{
	char* buffer_next = buffer_start;
	while (buffer_next < buffer_end && isspace(*buffer_next)) {
		++buffer_next;
	}
	return buffer_next;
}

Token GetNextToken(
	char* buffer_start,
	char* buffer_end,
	char** out_buffer_read)
{
	char* buffer_next = buffer_start;
	Token retval;

	if (*buffer_next == '(') {
		++buffer_next;
		retval = TOKEN_LPAREN;
	}
	else if (*buffer_next == ')') {
		++buffer_next;
		retval = TOKEN_RPAREN;
	}
	else if (isdigit(*buffer_next)) {
		++buffer_next;
		while(buffer_next < buffer_end && isdigit(*buffer_next))
			++buffer_next;
		retval = TOKEN_INTEGER_LITERAL;
	}
	else if (islower(*buffer_next) || isupper(*buffer_next)) {
		++buffer_next;
		while (buffer_next < buffer_end &&
			(islower(*buffer_next) || isupper(*buffer_next))) 
		{
			++buffer_next;
		}
		retval = TOKEN_IDENTIFIER;
	}
	else {
		assert(false, "unexpected character when reading token: %c",
			*buffer_next);
	}

	*out_buffer_read = buffer_next;
	return retval;
}

char* ConsumeToken(
	Token tok,
	char* buffer_start,
	char* buffer_end)
{
	char* buffer_next = buffer_start;
	buffer_next =  SkipWhitespace(buffer_start, buffer_end);
	char* buffer_read;
	Token found_tok = GetNextToken(buffer_next, buffer_end, &buffer_read);
	assert(found_tok == tok, "unexpected token, expected %d found %d",
		tok, found_tok);
	return buffer_read;
}

Identifier GetIdentifier(
	char* buffer_start,
	char* buffer_end,
	char** out_buffer_read)
{
	char* buffer_next = buffer_start;
	while (buffer_next < buffer_end &&
		(islower(*buffer_next) || isupper(*buffer_next))) 
	{
		++buffer_next;
	}
	Identifier id;
	id.string = buffer_start;
	id.string_len = buffer_next - buffer_start;
	*out_buffer_read = buffer_next;
	return id;
}

int GetIntegerLiteral(
	char* buffer_start,
	char* buffer_end,
	char** out_buffer_read)
{
	char* buffer_next = buffer_start;
	int len = 0;
	while (buffer_next < buffer_end && isdigit(*buffer_next)) {
		++buffer_next;
		++len;
	}
	buffer_next = buffer_start;
	int val = 0;
	int base = pow(10, len-1);
	while (buffer_next < buffer_end && isdigit(*buffer_next)) {
		val += (*buffer_next - '0') * base;
		base /= 10;
		++buffer_next;
	}
	*out_buffer_read = buffer_next;
	return val;
}

ASTNode* ParseExpression(
	char* buffer_start,
	char* buffer_end,
	char** out_buffer_read)
{
	ASTNode* ast = AllocASTNode();
	char* buffer_next = buffer_start;
	buffer_next = SkipWhitespace(buffer_start,buffer_end);
	assert(buffer_next < buffer_end, "end-of-file while expecting expression");

	char* tok_read;
	char* tok_start = buffer_next;
	Token tok = GetNextToken(buffer_next, buffer_end, &tok_read);
	buffer_next = tok_read;

	if (tok == TOKEN_INTEGER_LITERAL) {
		ast->type = AST_NODE_TYPE_INTEGER_LITERAL;
		char* lit_read;
		ast->literal_val = GetIntegerLiteral(tok_start,buffer_end,
			&lit_read);
		assert(tok_read == lit_read,
			"bad literal parse, tok_read=0x%p, lit_read=%p",
			tok_read, lit_read);
		char* dummy;
		assert(isspace(*buffer_next) || 
			GetNextToken(buffer_next, buffer_end, &dummy) == TOKEN_RPAREN,
			"delimiter expected after integer literal");
	}
	else if (tok == TOKEN_LPAREN) {
		buffer_next = SkipWhitespace(buffer_next, buffer_end);
		char* tok_read;
		Token tok = GetNextToken(buffer_next, buffer_end, &tok_read);
		assert(tok == TOKEN_IDENTIFIER, "unexpected token (wanted identifier)");
		ASTNodeType node_type;
		ASTNode *exp0, *exp1;
		char* id_read;
		Identifier id = GetIdentifier(buffer_next, buffer_end, &id_read);
		assert(id_read == tok_read, "tok and id size mismatch");
		buffer_next = tok_read;
		if (CompareIdentifier(id, "add")) {
			node_type = AST_NODE_TYPE_ADD;
			buffer_next = SkipWhitespace(buffer_next, buffer_end);
			char* buffer_read;
			exp0 = ParseExpression(buffer_next, buffer_end, &buffer_read);
			buffer_next = buffer_read;
			buffer_next = SkipWhitespace(buffer_next, buffer_end);
			exp1 = ParseExpression(buffer_next, buffer_end, &buffer_read);
			buffer_next = buffer_read;
		}
		else if (CompareIdentifier(id, "sub")) {
			node_type = AST_NODE_TYPE_SUB;
			buffer_next = SkipWhitespace(buffer_next, buffer_end);
			char* buffer_read;
			exp0 = ParseExpression(buffer_next, buffer_end, &buffer_read);
			buffer_next = buffer_read;
			buffer_next = SkipWhitespace(buffer_next, buffer_end);
			exp1 = ParseExpression(buffer_next, buffer_end, &buffer_read);
			buffer_next = buffer_read;
		}
		else if (CompareIdentifier(id, "mul")) {
			node_type = AST_NODE_TYPE_MUL;
			buffer_next = SkipWhitespace(buffer_next, buffer_end);
			char* buffer_read;
			exp0 = ParseExpression(buffer_next, buffer_end, &buffer_read);
			buffer_next = buffer_read;
			buffer_next = SkipWhitespace(buffer_next, buffer_end);
			exp1 = ParseExpression(buffer_next, buffer_end, &buffer_read);
			buffer_next = buffer_read;
		}
		else if (CompareIdentifier(id, "div")) {
			node_type = AST_NODE_TYPE_DIV;
			buffer_next = SkipWhitespace(buffer_next, buffer_end);
			char* buffer_read;
			exp0 = ParseExpression(buffer_next, buffer_end, &buffer_read);
			buffer_next = buffer_read;
			buffer_next = SkipWhitespace(buffer_next, buffer_end);
			exp1 = ParseExpression(buffer_next, buffer_end, &buffer_read);
			buffer_next = buffer_read;
		}
		else {
			assert(false, "unexpected literal");
		}
		buffer_next = ConsumeToken(TOKEN_RPAREN, buffer_next, buffer_end);
		ast->type = node_type;
		ast->exp0 = exp0;
		ast->exp1 = exp1;
	}
	else {
		assert(false, "unexpected token: %d", tok);
	}

	*out_buffer_read = buffer_next;
	return ast;
}

ASTNode* ParseBuffer(
	char* buffer,
	int buffer_len)
{
	char* buffer_read;
	char* buffer_start = buffer;
	char* buffer_end = buffer + buffer_len;
	ASTNode* ast = ParseExpression(
		buffer_start,
		buffer_end,
		&buffer_read);
	// TODO(jovanr): read multiple expressions
	buffer_read = SkipWhitespace(buffer_read, buffer_end);
	assert( buffer_end == buffer_read, "not all of the buffer was consumed");
	return ast;
}

}