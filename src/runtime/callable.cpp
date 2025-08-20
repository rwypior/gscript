#include "runtime/callable.hpp"

namespace gscript
{
	ScriptCallable::ScriptCallable(ScriptScope &scope)
		: scope(scope)
	{
	}

	ScriptScope &ScriptCallable::getScope()
	{
		return this->scope;
	}
}