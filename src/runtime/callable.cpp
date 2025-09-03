#include "runtime/callable.hpp"

namespace gscript
{
	ScriptCallable::ScriptCallable(ScriptScopeBase& scope)
		: scope(scope)
	{
	}

	ScriptScopeBase& ScriptCallable::getScope()
	{
		return this->scope;
	}
}