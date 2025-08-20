#ifndef _h_gscript_array_initializer
#define _h_gscript_array_initializer

#include "defs.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "funcCall.hpp"

namespace gscript
{
	class ScriptArrayInitializer : public ScriptCallable
	{
	public:
		ScriptArrayInitializer(ScriptScope &scope, std::vector<std::unique_ptr<ScriptStatement>>&& statements);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const;

	protected:
		const ScriptType* type = nullptr;
		std::vector<std::unique_ptr<ScriptStatement>> statements;
	};
}

#endif