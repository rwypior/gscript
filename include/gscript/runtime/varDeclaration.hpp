#ifndef _h_gscript_vardeclaration
#define _h_gscript_vardeclaration

#include "callable.hpp"
#include "scriptValue.hpp"
#include "statement.hpp"
#include "entityLink.hpp"

#include <string>
#include <memory>

namespace gscript
{
	class ScriptScopeBase;
	class ScriptScope;
	class ScriptVariable;
	class VariableAccessor;

	class ScriptVarDeclaration : public ScriptCallable
	{
	public:

		ScriptVarDeclaration(ScriptScope &scope, ScriptVariable &var, std::unique_ptr<ScriptStatement> &&statement);
		ScriptVarDeclaration(ScriptScope &scope, std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptStatement> &&statement);

		void setInstance(ScriptClassInstance &instance);

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &p = CALLABLE_PARAMS_T()) override;

		virtual const std::shared_ptr<ScriptType> getType() const override;

		const std::unique_ptr<ScriptCallable>& getStatement() const;
		std::unique_ptr<ScriptCallable>& getStatement();

		std::string _name() const
		{
			return "VarDeclaration";
		}

	protected:
		std::unique_ptr<VariableAccessor> accessor;
		std::unique_ptr<ScriptCallable> statement;
	};

	class ScriptFieldDeclaration : public ScriptVarDeclaration
	{
	public:
		ScriptFieldDeclaration(ScriptScope& scope, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptStatement>&& statement);

		void instantiate(ScriptScopeBase& instance);

	private:
		std::string name;
		const std::shared_ptr<ScriptType> type;
	};
}

#endif