#ifndef _h_gscript_for
#define _h_gscript_for

#include <string>
#include <vector>
#include <memory>

#include "variable.hpp"
#include "varDeclaration.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "scope.hpp"
#include "runnable.hpp"

namespace gscript
{
	class ParserFor;

	class ScriptFor : public ScriptRunnable, public ScriptCallable
	{
	private:
		static const ScriptType *returnType;

	public:
		ScriptVarDeclaration * vardecl;
		ScriptStatement *condition;
		ScriptStatement *progress;

		ScriptFor(ScriptScope &scope, const ScriptVarDeclaration &vardecl, const ScriptStatement &condition, const ScriptStatement &progress);
		ScriptFor(ScriptScope &scope, const ParserFor &pfor);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;
	};
}

#endif