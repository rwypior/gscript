#include "runtime/if.hpp"
#include "defs.hpp"

namespace gscript
{
	const ScriptType *ScriptIf::returnType = new ScriptType(VALUE_TYPE_T::VT_NULL);

	ScriptIf::ScriptIf(ScriptScope &scope, std::unique_ptr<ScriptStatement>&& condition, std::vector<std::shared_ptr<ScriptCallable>>&& statements)
		: ScriptCallable(scope)
		, ScriptExecutiveBlock(std::move(statements))
		, condition(std::move(condition))
	{
	}

	ScriptValue *ScriptIf::run(const CALLABLE_PARAMS_T &c)
	{
		if (!this->condition || this->condition->run()->boolean().getValue())
			return ScriptExecutiveBlock::execute();
		else if (this->selse)
			return this->selse->run();

		return SCR_NULL;
	}

	const ScriptType *ScriptIf::getType() const
	{
		return ScriptIf::returnType;
	}
}