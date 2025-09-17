#ifndef _h_gscript_for
#define _h_gscript_for

#include "variable.hpp"
#include "varDeclaration.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "scope.hpp"
#include "runnable.hpp"
#include "executiveBlock.hpp"
#include "lib.hpp"

namespace gscript
{
	class ParserFor;

	class ScriptFor : public ScriptExecutiveBlock, public ScriptCallable
	{
	public:
		SCRIPT_API ScriptFor(
			ScriptScope &scope, 
			std::unique_ptr<ScriptVarDeclaration>&& vardecl, 
			std::unique_ptr<ScriptStatement>&& condition, 
			std::unique_ptr<ScriptStatement>&& progress,
			std::vector<std::shared_ptr<ScriptCallable>>&& statements = {}
		);

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		SCRIPT_API const std::shared_ptr<ScriptType> getType() const override;

	private:
		std::unique_ptr<ScriptVarDeclaration> vardecl = nullptr;
		std::unique_ptr<ScriptStatement> condition = nullptr;
		std::unique_ptr<ScriptStatement> progress = nullptr;
	};
}

#endif