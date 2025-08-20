#ifndef _h_gscript_while
#define _h_gscript_while

#include "executiveBlock.hpp"
#include "callable.hpp"

#include "scriptValue.hpp"
#include "scope.hpp"

#include <vector>
#include <memory>

namespace gscript
{
	class ParserWhile;
	class ScriptStatement;

	class ScriptWhile : public ScriptExecutiveBlock, public ScriptCallable
	{
	public:
		ScriptWhile(ScriptScope& scope, std::unique_ptr<ScriptStatement>&& condition, std::vector<std::shared_ptr<ScriptCallable>>&& statements = {});
		//ScriptWhile(ScriptScope &scope, const ParserWhile &pwhile);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;

	private:
		static const ScriptType *returnType;

		std::unique_ptr<ScriptStatement> condition;
	};
}

#endif