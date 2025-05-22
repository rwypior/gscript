#ifndef _h_gscript_conditionaloperator
#define _h_gscript_conditionaloperator

#include "callable.hpp"
#include "statement.hpp"
#include "EntityPath.hpp"
#include "funcParam.hpp"
#include "pConditionalOperator.hpp"
#include "defs.hpp"

#include <string>
#include <vector>

namespace gscript
{
	class ScriptScope;
	class ScriptFunction;
	class ScriptStatement;

	class ScriptMethodCall;

	class ScriptConditionalOperator : public ScriptCallable
	{
	public:
		ScriptStatement* cond;
		ScriptStatement* ontrue;
		ScriptStatement* onfalse;

		ScriptConditionalOperator(ScriptScope& scope, ParserConditionalOperator* cond);
		ScriptConditionalOperator(ScriptScope& scope, ScriptStatement* cond, ScriptStatement* ontrue, ScriptStatement* onfalse);
		ScriptConditionalOperator(ScriptScope& scope, ScriptStatement* cond, ScriptStatement* onfalse);

		virtual ScriptValue* run(const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType* getType() const override;
	};
}

#endif