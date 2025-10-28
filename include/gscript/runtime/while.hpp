#ifndef _h_gscript_while
#define _h_gscript_while

#include "executiveBlock.hpp"
#include "callable.hpp"

#include "scriptValue.hpp"
#include "scope.hpp"

#include <vector>
#include <memory>

namespace gscript
{
	class ParserWhile;
	class ScriptStatement;

	class ScriptWhile : public ScriptExecutiveBlock, public ScriptCallable
	{
	public:
		ScriptWhile(const ScriptWhile& b);
		ScriptWhile(std::unique_ptr<ScriptStatement>&& condition, std::vector<std::unique_ptr<ScriptCallable>>&& statements = {});

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const std::shared_ptr<ScriptType> getType() const override;

	private:
		std::unique_ptr<ScriptStatement> condition;
	};
}

#endif