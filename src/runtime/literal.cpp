#include "runtime/literal.hpp"
#include "runtime/scriptValue.hpp"
#include "defs.hpp"
#include "runtimeException.hpp"
#include "debug.hpp"

namespace gscript
{
	ScriptLiteral::ScriptLiteral(const ScriptLiteral& literal)
		: val(literal.val->clone())
	{
	}

	ScriptLiteral::ScriptLiteral(ScriptScopeBase& scope, std::unique_ptr<ScriptValue> &&val)
		//: ScriptCallable(scope)
		: val(std::move(val))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptLiteral::clone()
	{
		return std::make_unique<ScriptLiteral>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptLiteral::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->val->clone();
	}

	const std::shared_ptr<ScriptType> ScriptLiteral::getType() const
	{
		return this->val->getType();
	}
}