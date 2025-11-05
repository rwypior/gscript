#include "gscript/runtime/if.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/defs.hpp"
#include "gscript/util.hpp"

#include <memory>

namespace gscript
{
	ScriptIf::ScriptIf(const ScriptIf& sif)
		: ScriptScope(sif)
		, ScriptExecutiveBlock(sif)
		, condition(sif.condition->clone())
		, selse(sif.selse ? static_unique_pointer_cast<ScriptIf>(sif.selse->clone()) : nullptr)
	{
	}

	ScriptIf::ScriptIf(ScriptScope &scope, std::unique_ptr<ScriptStatement>&& condition, std::unique_ptr<ScriptIf>&& selse, std::vector<std::unique_ptr<ScriptCallable>>&& statements)
		: ScriptScope(&scope)
		, ScriptExecutiveBlock(std::move(statements))
		, condition(std::move(condition))
		, selse(std::move(selse))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptIf::clone()
	{
		return std::make_unique<ScriptIf>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptIf::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		if (!this->condition || this->condition->run(scope)->boolean().getValue())
			return ScriptExecutiveBlock::execute(*this);
		else if (this->selse)
			return this->selse->run(scope);

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