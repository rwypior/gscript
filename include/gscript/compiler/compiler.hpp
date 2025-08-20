#ifndef _h_gscript_compiler
#define _h_gscript_compiler

#include <memory>

namespace gscript
{
	class ParserCallArglist;
	class ParserNamespace;
	class ParserClass;
	class ParserFunction;
	class ParserMethod;
	class ParserValue;
	class ParserLiteral;
	class ParserStatement;
	class ParserBlockBody;
	class ParserOperator;
	class ParserVar;
	class ParserVarDeclaration;
	class ParserNew;
	class ParserFuncCall;
	class ParserArrayInitializer;
	class ParserIf;
	class ParserWhile;
	class ParserFor;
	class ParserReturn;
	//class ParserConditionalOperator;

	class FunctionParameterContainer;
	class ScriptScope;
	class ScriptNamespace;
	class ScriptClass;
	class ScriptFunction;
	class ScriptMethod;
	class ScriptValue;
	class ScriptLiteral;
	class ScriptStatement;
	class ScriptExecutiveBlock;
	class ScriptOperator;
	class ScriptVarRead;
	class ScriptVarDeclaration;
	class ScriptNew;
	class ScriptFuncCall;
	class ScriptArrayInitializer;
	class ScriptIf;
	class ScriptWhile;
	class ScriptFor;
	class ScriptReturn;
	//class ScriptConditionalOperator;

	class Compiler
	{
	public:
		std::unique_ptr<FunctionParameterContainer> compileParameterContainer(ScriptScope* scope, const ParserCallArglist& arglist);

		std::unique_ptr<ScriptNamespace> compileNamespace(ScriptScope* scope, const ParserNamespace& entry);
		std::unique_ptr<ScriptClass> compileClass(ScriptNamespace* scope, const ParserClass& pclass);
		std::unique_ptr<ScriptFunction> compileFunction(ScriptScope* scope, const ParserFunction& pfunc);
		std::unique_ptr<ScriptMethod> compileMethod(ScriptScope* scope, const ParserMethod& pfunc);
		std::unique_ptr<ScriptValue> compileValue(const ParserLiteral& pLiteral);
		std::unique_ptr<ScriptStatement> compileStatement(ScriptScope* scope, const ParserStatement& pstatement);
		std::unique_ptr<ScriptExecutiveBlock> compileExecutiveBlock(ScriptScope* scope, const ParserBlockBody& pblock);
		std::unique_ptr<ScriptOperator> compileOperator(ScriptScope* scope, const ParserOperator& pOperator);
		std::unique_ptr<ScriptVarRead> compileVarRead(ScriptScope* scope, const ParserVar& pVar);
		std::unique_ptr<ScriptNew> compileNewCall(ScriptScope* scope, const ParserNew& fcall);
		std::unique_ptr<ScriptFuncCall> compileFuncCall(ScriptScope* scope, const ParserFuncCall& fcall);
		std::unique_ptr<ScriptArrayInitializer> compileArrayInitializer(ScriptScope* scope, const ParserArrayInitializer& initializer);

		std::unique_ptr<ScriptVarDeclaration> compileVarDeclaration(ScriptScope* scope, const ParserVarDeclaration& pVar);
		std::unique_ptr<ScriptIf> compileIf(ScriptScope* scope, const ParserIf& pIf);
		std::unique_ptr<ScriptWhile> compileWhile(ScriptScope* scope, const ParserWhile& pWhile);
		std::unique_ptr<ScriptFor> compileFor(ScriptScope* scope, const ParserFor& pFor);
		std::unique_ptr<ScriptReturn> compileReturn(ScriptScope* scope, const ParserReturn& pReturn);
		//std::unique_ptr<ScriptConditionalOperator> compileConditionalOperator(ScriptScope* scope, const ParserConditionalOperator& pReturn);
	};
}

#endif