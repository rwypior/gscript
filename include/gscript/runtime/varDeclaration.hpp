#ifndef _h_gscript_vardeclaration
#define _h_gscript_vardeclaration

#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/entityLink.hpp"

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
		SCRIPT_API ScriptVarDeclaration(const ScriptVarDeclaration& b);
		SCRIPT_API ScriptVarDeclaration(ScriptScope &scope, ScriptVariable &var, std::unique_ptr<ScriptStatement> &&statement);
		SCRIPT_API ScriptVarDeclaration(std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptStatement> &&statement);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API void setInstance(ScriptClassInstance &instance);

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &p = CALLABLE_PARAMS_T()) override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;

		SCRIPT_API const std::unique_ptr<ScriptCallable>& getStatement() const;
		SCRIPT_API std::unique_ptr<ScriptCallable>& getStatement();

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
		SCRIPT_API ScriptFieldDeclaration(const ScriptFieldDeclaration& b);
		SCRIPT_API ScriptFieldDeclaration(const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptStatement>&& statement);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API void instantiate(ScriptScopeBase& instance);

	private:
		std::string name;
		const std::shared_ptr<ScriptType> type;
	};
}

#endif