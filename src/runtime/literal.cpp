#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/defs.hpp"
#include "gscript/runtimeException.hpp"
#include "gscript/debug.hpp"

namespace gscript
{
	ScriptLiteral::ScriptLiteral(const ScriptLiteral& literal)
		: val(literal.val->clone())
	{
	}

	ScriptLiteral::ScriptLiteral(std::unique_ptr<ScriptValue> &&val)
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