#ifndef _h_gscript_vardeclaration
#define _h_gscript_vardeclaration

#include "callable.hpp"
#include "scriptValue.hpp"
#include "statement.hpp"
#include "entityLink.hpp"
//#include "intermediary.hpp"

#include <string>

namespace gscript
{
	class ScriptScopeBase;
	class ScriptScope;
	class ScriptVariable;

	class ScriptVarDeclaration : public ScriptCallable//, public Intermediary
	{
	public:

		ScriptVarDeclaration(ScriptScope &scope, ScriptVariable &var, std::unique_ptr<ScriptStatement> &&statement);
		ScriptVarDeclaration(ScriptScope &scope, VariableAccessor accessor, std::unique_ptr<ScriptStatement> &&statement);
		//ScriptVarDeclaration(ScriptScope &scope, ParserVarDeclaration &pvardecl);

		//virtual void setup(ScriptScope& scope) override;

		void setInstance(ScriptClassInstance &instance);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &p = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;

		std::string _name() const
		{
			return "VarDeclaration";
		}

	protected:
		VariableAccessor accessor;
		//EntityLink<ScriptVariable&> *var = nullptr;
		std::unique_ptr<ScriptStatement> statement;
	};

	class ScriptFieldDeclaration : public ScriptVarDeclaration
	{
	public:
		ScriptFieldDeclaration(ScriptScope& scope, const std::string& name, const ScriptType* type, std::unique_ptr<ScriptStatement>&& statement);

		void instantiate(ScriptScopeBase& instance);

	private:
		std::string name;
		const ScriptType* type;
		std::unique_ptr<ScriptStatement> statement;
	};

	class ScriptVarDeclarationPrototype : public ScriptCallablePrototype
	{
	public:
		ScriptVarDeclarationPrototype(ScriptScope& scope, const std::string& name, const ScriptType* type, std::unique_ptr<ScriptStatement>&& statement);

		virtual std::unique_ptr<ScriptCallable> build() override;

	private:
		std::string name;
		const ScriptType* type;
		std::unique_ptr<ScriptStatement> statement;
	};
}

#endif