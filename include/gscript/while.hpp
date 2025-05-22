#ifndef _h_gscript_while
#define _h_gscript_while

#include <memory>

#include "runnable.hpp"
#include "callable.hpp"

#include "scriptValue.hpp"
#include "scope.hpp"

namespace gscript
{
	class ParserWhile;
	class ScriptStatement;

	class ScriptWhile : public ScriptRunnable, public ScriptCallable
	{
	private:
		static const ScriptType *returnType;

	public:
		ScriptStatement * condition;

		ScriptWhile(ScriptScope &scope, const ScriptStatement &condition);
		ScriptWhile(ScriptScope &scope, const ParserWhile &pwhile);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;
	};
}

#endif