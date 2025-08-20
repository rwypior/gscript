#ifndef _h_gscript_vardeclaration
#define _h_gscript_vardeclaration

#include "callable.hpp"
#include "scriptValue.hpp"
#include "statement.hpp"
#include "entityLink.hpp"
//#include "intermediary.hpp"

#include <string>

namespace gscript
{
	class ScriptScope;
	class ScriptVariable;

	class ScriptVarDeclaration : public ScriptCallable//, public Intermediary
	{
	public:

		ScriptVarDeclaration(ScriptScope &scope, ScriptVariable &var, std::unique_ptr<ScriptStatement> &&statement);
		ScriptVarDeclaration(ScriptScope &scope, EntityLink<ScriptVariable&> *link, std::unique_ptr<ScriptStatement> &&statement);
		//ScriptVarDeclaration(ScriptScope &scope, ParserVarDeclaration &pvardecl);

		//virtual void setup(ScriptScope& scope) override;

		void setInstance(ScriptClassInstance &instance);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &p = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;

		std::string _name() const
		{
			return "VarDeclaration";
		}

	private:
		EntityLink<ScriptVariable&> *var = nullptr;
		std::unique_ptr<ScriptStatement> statement;
	};
}

#endif