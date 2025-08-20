#include "runtime/return.hpp"

namespace gscript
{
	const ScriptType *ScriptReturn::returnType = new ScriptType(VALUE_TYPE_T::VT_NULL);

	ScriptReturn::ScriptReturn(ScriptScope &scope, std::unique_ptr<ScriptStatement>&& statement)
		: ScriptCallable(scope)
		, statement(std::move(statement))
	{
	}

	ScriptValue *ScriptReturn::run(const CALLABLE_PARAMS_T &c)
	{
		return this->statement->run();
	}

	const ScriptType *ScriptReturn::getType() const
	{
		return ScriptReturn::returnType;
	}
}