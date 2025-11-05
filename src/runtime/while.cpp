#include "gscript/runtime/while.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/util.hpp"

namespace gscript
{
	ScriptWhile::ScriptWhile(const ScriptWhile& b)
		: ScriptScope(b)
		, ScriptExecutiveBlock(b)
		, condition(static_unique_pointer_cast<ScriptStatement>(b.condition->clone()))
	{
	}

	ScriptWhile::ScriptWhile(ScriptScope& scope, std::unique_ptr<ScriptStatement>&& condition, std::vector<std::unique_ptr<ScriptCallable>>&& statements)
		: ScriptScope(&scope)
		, ScriptExecutiveBlock(std::move(statements))
		, condition(std::move(condition))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptWhile::clone()
	{
		return std::make_unique<ScriptWhile>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptWhile::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		while (this->condition->run(scope)->boolean().getValue())
			ScriptExecutiveBlock::execute(*this);

		return ScriptType::null();
	}

	const std::shared_ptr<ScriptType> ScriptWhile::getType() const
	{
		return ScriptType::nulltype();
	}
}