#ifndef _h_script_runnable
#define _h_script_runnable

#include "callable.hpp"
#include "scope.hpp"

#include <vector>

namespace gscript
{
	class ScriptValue;
	class ParserBlockBody;

	class ScriptRunnable : public ScriptScope
	{
	public:
		SCRIPT_API ScriptRunnable(ScriptScope *parentScope);

		std::vector<std::shared_ptr<ScriptCallable>> statements;

		SCRIPT_API virtual ScriptValue *run();
		SCRIPT_API void setup(const ParserBlockBody &pbody);
	};
}

#endif