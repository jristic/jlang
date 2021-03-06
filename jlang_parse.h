
namespace Parse
{
	enum ASTNodeType {
		AST_NODE_TYPE_INTEGER_LITERAL,
		AST_NODE_TYPE_ADD,
		AST_NODE_TYPE_SUB,
		AST_NODE_TYPE_MUL,
		AST_NODE_TYPE_DIV,
	};

	struct ASTNode {
		ASTNodeType type;
		union {
			struct {
				ASTNode *exp0, *exp1;
			};
			int literal_val;
		};
	};

	Vector<ASTNode*> ParseBuffer(
		char* buffer,
		int buffer_len);
}