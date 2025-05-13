#ifndef _h_script_if
#define _h_script_if

#include <string>
#include <vector>
#include <memory>

#include "variable.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "scope.hpp"
#include "runnable.hpp"

namespace gscript
{
	class ParserFunction;
	class ParserIf;
	class ParserElse;

	class ScriptIf : public ScriptRunnable, public ScriptCallable
	{
	private:
		static const ScriptType *returnType;

	public:
		ScriptStatement * condition;
		ScriptIf *selse = NULL;

		ScriptIf(ScriptScope &scope, const ScriptStatement &condition);
		ScriptIf(ScriptScope &scope, const ParserIf &pif, ScriptScope &parentScope);
		ScriptIf(ScriptScope &scope, const ParserElse &pelse, ScriptScope &parentScope);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		//void setup(const ParserBlockBody &pbody);

		virtual const ScriptType *getType() const override;
	};
}

#endif