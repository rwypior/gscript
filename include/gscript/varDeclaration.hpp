#ifndef _h_gscript_vardeclaration
#define _h_gscript_vardeclaration

#include "callable.hpp"
#include "scriptValue.hpp"
#include "statement.hpp"

#include "pVarDeclaration.hpp"

#include "entityLink.hpp"

#include <vector>
#include <memory>
#include <string>

namespace gscript
{
	class ScriptScope;
	class ScriptVariable;

	class ScriptVarDeclaration : public ScriptCallable
	{
	public:
		//ScriptVariable &var;
		EntityLink<ScriptVariable&> *var = nullptr;
		ScriptStatement statement;

		ScriptVarDeclaration(ScriptScope &scope, ScriptVariable &var, const ScriptStatement &statement);
		ScriptVarDeclaration(ScriptScope &scope, EntityLink<ScriptVariable&> *link, const ScriptStatement &statement);
		ScriptVarDeclaration(ScriptScope &scope, ParserVarDeclaration &pvardecl);

		void setInstance(ScriptClassInstance &instance);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &p = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;

		std::string _name() const
		{
			return "VarDeclaration";
		}
	};
}

#endif