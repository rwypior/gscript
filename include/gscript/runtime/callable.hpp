#ifndef _h_gscript_callable
#define _h_gscript_callable

#include "scriptValue.hpp"
#include "lib.hpp"
#include "runtimeException.hpp"
#include "prototype.hpp"

#include <memory>
#include <vector>

namespace gscript
{
	class ScriptScope;

	class ScriptCallable
	{
	public:
		SCRIPT_API ScriptCallable(ScriptScopeBase& scope);

		SCRIPT_API virtual const ScriptType *getType() const = 0;

		SCRIPT_API virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) = 0;

		// Post-compilation stage - build prototypes, and do additional steps
		SCRIPT_API virtual void setup() {};

		SCRIPT_API ScriptScopeBase& getScope();

	protected:
		ScriptScopeBase& scope;
	};

	// Partially created callable entity - not usable in raw form.
	// In order to use - it must first be built into an actual object after compilation stage
	// by calling build() function
	class ScriptCallablePrototype : public ScriptCallable, public Prototype<ScriptCallable>
	{
	public:
		using ScriptCallable::ScriptCallable;

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> build() override = 0;

		SCRIPT_API virtual const ScriptType* getType() const
		{
			throw RuntimeException("Attempted to call callable prototype");
		}

		SCRIPT_API virtual ScriptValue* run(const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override
		{
			throw RuntimeException("Attempted to call callable prototype");
		}
	};
}

#endif