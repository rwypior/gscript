#ifndef _h_script_statement
#define _h_script_statement

#include <vector>
#include <memory>

#include "callable.hpp"

#include "lib.hpp"

namespace gscript
{
	class ParserStatement;
	class ScriptValue;
	class ScriptOperator;

	class ScriptStatement : public ScriptCallable
	{
	protected:
		/// <return>count of processed callables</return>
		SCRIPT_API int resolveOperations(
			std::vector<ScriptCallable*>::reverse_iterator begin,
			std::vector<ScriptCallable*>::reverse_iterator end,
			ScriptCallable *&result,
			unsigned int depth = 0
		);

		SCRIPT_API void assignReferences(ScriptCallable *&entry, ScriptScope *scope, bool member);
		SCRIPT_API void setupOperator(ScriptOperator *oper);

	public:
		ScriptCallable *callable;

		SCRIPT_API ScriptStatement(ScriptScope &scope, const ParserStatement &pstatement);
		SCRIPT_API ScriptStatement(ScriptScope &scope);

		SCRIPT_API virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		SCRIPT_API virtual const ScriptType *getType() const override;
	};
}

#endif