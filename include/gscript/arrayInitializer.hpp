#ifndef _h_gscript_array_initializer
#define _h_gscript_array_initializer

#include "defs.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "funcCall.hpp"
#include "pArrayInitializer.hpp"

#include <string>

namespace gscript
{
	class ScriptArrayInitializer : public ScriptCallable
	{
	public:
		typedef std::vector<ScriptStatement*> STATEMENT_CONTAINER_T;

	public:
		ScriptArrayInitializer(ScriptScope &scope, ParserArrayInitializer &arrayInitializer);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const;

	protected:
		const ScriptType* type = nullptr;
		STATEMENT_CONTAINER_T statements;
	};
}

#endif