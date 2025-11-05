#ifndef _h_gscript_compiler
#define _h_gscript_compiler

#include "gscript/lib.hpp"

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
	class ParserElse;
	class ParserWhile;
	class ParserFor;
	class ParserReturn;
	//class ParserConditionalOperator;

	class ScriptCallable;
	class ScriptScopeBase;
	class ScriptScope;
	class FunctionParameterContainer;
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
		SCRIPT_API std::unique_ptr<FunctionParameterContainer> compileParameterContainer(ScriptScope* scope, const ParserCallArglist& arglist);

		SCRIPT_API std::unique_ptr<ScriptNamespace> compileNamespace(ScriptScope* scope, const ParserNamespace& entry);
		SCRIPT_API std::unique_ptr<ScriptClass> compileClass(ScriptNamespace* scope, const ParserClass& pclass);
		SCRIPT_API std::unique_ptr<ScriptFunction> compileFunction(ScriptScope* scope, const ParserFunction& pfunc);
		SCRIPT_API std::unique_ptr<ScriptMethod> compileMethod(ScriptScope* scope, const ParserMethod& pfunc);
		SCRIPT_API std::unique_ptr<ScriptValue> compileValue(const ParserLiteral& pLiteral);
		SCRIPT_API std::unique_ptr<ScriptStatement> compileStatement(ScriptScope* scope, const ParserStatement& pstatement, bool topmostVarReadReference = true);
		SCRIPT_API std::unique_ptr<ScriptExecutiveBlock> compileExecutiveBlock(ScriptScope* scope, const ParserBlockBody& pblock);
		SCRIPT_API std::unique_ptr<ScriptOperator> compileOperator(const ParserOperator& pOperator);
		SCRIPT_API std::unique_ptr<ScriptCallable> compileVarRead(ScriptScope* scope, const ParserVar& pVar);
		SCRIPT_API std::unique_ptr<ScriptNew> compileNewCall(ScriptScope* scope, const ParserNew& fcall);
		SCRIPT_API std::unique_ptr<ScriptCallable> compileFuncCall(ScriptScope* scope, const ParserFuncCall& fcall);
		SCRIPT_API std::unique_ptr<ScriptArrayInitializer> compileArrayInitializer(ScriptScope* scope, const ParserArrayInitializer& initializer);

		SCRIPT_API std::unique_ptr<ScriptVarDeclaration> compileVarDeclaration(ScriptScope* scope, const ParserVarDeclaration& pVar);
		SCRIPT_API std::unique_ptr<ScriptIf> compileIf(ScriptScope* scope, const ParserIf& pIf);
		SCRIPT_API std::unique_ptr<ScriptIf> compileElse(ScriptScope* scope, const ParserElse& pElse);
		SCRIPT_API std::unique_ptr<ScriptWhile> compileWhile(ScriptScope* scope, const ParserWhile& pWhile);
		SCRIPT_API std::unique_ptr<ScriptFor> compileFor(ScriptScope* scope, const ParserFor& pFor);
		SCRIPT_API std::unique_ptr<ScriptReturn> compileReturn(ScriptScope* scope, const ParserReturn& pReturn);
		//std::unique_ptr<ScriptConditionalOperator> compileConditionalOperator(ScriptScope* scope, const ParserConditionalOperator& pReturn);

		void SCRIPT_API finalize(ScriptNamespace& ns);
		void SCRIPT_API finalizeScope(ScriptScopeBase& scope);
		void SCRIPT_API finalizeCallable(std::shared_ptr<ScriptCallable>& callable, ScriptScopeBase& scope);
		void SCRIPT_API finalizeCallable(std::unique_ptr<ScriptCallable>& callable, ScriptScopeBase& scope);
	};
}

#endif