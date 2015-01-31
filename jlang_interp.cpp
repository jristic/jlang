
namespace Interp
{

InterpResult InterpAST(Parse::ASTNode* ast)
{
	InterpResult result;
	switch (ast->type) 
	{
		case Parse::AST_NODE_TYPE_INTEGER_LITERAL:
		{
			result.intResult = ast->literal_val;
			break;
		}
		case Parse::AST_NODE_TYPE_ADD:
		{
			InterpResult val0 = InterpAST(ast->exp0);
			InterpResult val1 = InterpAST(ast->exp1);
			result.intResult = val0.intResult + val1.intResult;
			break;
		}
		case Parse::AST_NODE_TYPE_SUB:
		{
			InterpResult val0 = InterpAST(ast->exp0);
			InterpResult val1 = InterpAST(ast->exp1);
			result.intResult = val0.intResult - val1.intResult;
			break;
		}
		case Parse::AST_NODE_TYPE_MUL:
		{
			InterpResult val0 = InterpAST(ast->exp0);
			InterpResult val1 = InterpAST(ast->exp1);
			result.intResult = val0.intResult * val1.intResult;
			break;
		}
		case Parse::AST_NODE_TYPE_DIV:
		{
			InterpResult val0 = InterpAST(ast->exp0);
			InterpResult val1 = InterpAST(ast->exp1);
			result.intResult = val0.intResult / val1.intResult;
			break;
		}
		default:
			assert(false, "unknown ast node type");
			break;
	}
	return result;
}

}