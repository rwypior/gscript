#include "return.hpp"

namespace gscript
{
	const ScriptType *ScriptReturn::returnType = new ScriptType(VALUE_TYPE_T::VT_NULL);

	ScriptReturn::ScriptReturn(ScriptScope &scope, const ScriptStatement &statement)
		:ScriptCallable(scope),
		statement(statement)
	{
	}

	ScriptValue *ScriptReturn::run(const CALLABLE_PARAMS_T &c)
	{
		return this->statement.run();
	}

	const ScriptType *ScriptReturn::getType() const
	{
		return ScriptReturn::returnType;
	}
}