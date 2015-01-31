
namespace Interp
{

struct InterpResult {
	int intResult;
};

InterpResult InterpAST(Parse::ASTNode* ast);

}