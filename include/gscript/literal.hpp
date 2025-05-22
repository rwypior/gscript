#ifndef _h_gscript_literal
#define _h_gscript_literal

#include "callable.hpp"
#include "pLiteral.hpp"
#include "scriptValue.hpp"

namespace gscript
{
	class ScriptLiteral : public ScriptCallable
	{
	public:
		ScriptValue * val;

		ScriptLiteral(ScriptScope &scope, ScriptValue *val);
		ScriptLiteral(ScriptScope &scope, ParserLiteral pliteral);
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		virtual const ScriptType *getType() const override;
	};
}

#endif