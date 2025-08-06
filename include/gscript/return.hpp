#ifndef _h_gscript_return
#define _h_gscript_return

#include "callable.hpp"
#include "statement.hpp"

namespace gscript
{
	class ScriptReturn : public ScriptCallable
	{
	public:
		ScriptStatement statement;

		ScriptReturn(ScriptScope &scope, const ScriptStatement &statement);
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		virtual const ScriptType *getType() const override;

	private:
		static const ScriptType *returnType;
	};
}

#endif