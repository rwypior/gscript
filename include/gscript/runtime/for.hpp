#ifndef _h_gscript_for
#define _h_gscript_for

#include "gscript/lib.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/scope.hpp"
#include "gscript/runtime/executiveBlock.hpp"

namespace gscript
{
	class ParserFor;

	class ScriptFor : public ScriptScope, public ScriptExecutiveBlock, public ScriptCallable
	{
	public:
		SCRIPT_API ScriptFor(const ScriptFor& b);
		SCRIPT_API ScriptFor(
			ScriptScope& scope,
			std::unique_ptr<ScriptVarDeclaration>&& vardecl, 
			std::unique_ptr<ScriptStatement>&& condition, 
			std::unique_ptr<ScriptStatement>&& progress,
			std::vector<std::unique_ptr<ScriptCallable>>&& statements = {}
		);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		SCRIPT_API const std::shared_ptr<ScriptType> getType() const override;

	private:
		std::unique_ptr<ScriptVarDeclaration> vardecl = nullptr;
		std::unique_ptr<ScriptStatement> condition = nullptr;
		std::unique_ptr<ScriptStatement> progress = nullptr;
	};
}

#endif