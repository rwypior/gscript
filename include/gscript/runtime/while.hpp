#ifndef _h_gscript_while
#define _h_gscript_while

#include "gscript/runtime/executiveBlock.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/scope.hpp"

#include <vector>
#include <memory>

namespace gscript
{
	class ParserWhile;
	class ScriptStatement;

	class ScriptWhile : public ScriptScope, public ScriptExecutiveBlock, public ScriptCallable
	{
	public:
		SCRIPT_API ScriptWhile(const ScriptWhile& b);
		SCRIPT_API ScriptWhile(ScriptScope& scope, std::unique_ptr<ScriptStatement>&& condition, std::vector<std::unique_ptr<ScriptCallable>>&& statements = {});

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;

	private:
		std::unique_ptr<ScriptStatement> condition;
	};
}

#endif