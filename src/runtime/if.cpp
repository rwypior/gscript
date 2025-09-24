#include "runtime/if.hpp"
#include "defs.hpp"

#include <memory>

namespace gscript
{
	ScriptIf::ScriptIf(ScriptScope &scope, std::unique_ptr<ScriptStatement>&& condition, std::unique_ptr<ScriptIf>&& selse, std::vector<std::shared_ptr<ScriptCallable>>&& statements)
		: ScriptCallable(scope)
		, ScriptExecutiveBlock(std::move(statements))
		, condition(std::move(condition))
		, selse(std::move(selse))
	{
	}

	std::unique_ptr<ScriptValue> ScriptIf::run(const CALLABLE_PARAMS_T &c)
	{
		if (!this->condition || this->condition->run()->boolean().getValue())
			return ScriptExecutiveBlock::execute();
		else if (this->selse)
			return this->selse->run();

		return ScriptType::null();
	}

	const std::shared_ptr<ScriptType> ScriptIf::getType() const
	{
		return ScriptType::nulltype();
	}

	const std::unique_ptr<ScriptCallable>& ScriptIf::getCondition() const
	{
		return this->condition;
	}

	std::unique_ptr<ScriptCallable>& ScriptIf::getCondition()
	{
		return this->condition;
	}
}