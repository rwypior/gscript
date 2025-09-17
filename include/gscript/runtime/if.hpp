#ifndef _h_gscript_if
#define _h_gscript_if

#include "variable.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "scope.hpp"
#include "executiveBlock.hpp"

#include <vector>
#include <memory>

namespace gscript
{
	class ParserFunction;
	class ParserIf;
	class ParserElse;

	class ScriptIf : public ScriptExecutiveBlock, public ScriptCallable
	{
	public:
		ScriptIf(ScriptScope& scope, std::unique_ptr<ScriptStatement>&& condition, std::unique_ptr<ScriptIf>&& selse = {}, std::vector<std::shared_ptr<ScriptCallable>>&& statements = {});

		virtual std::unique_ptr<ScriptValue> run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const std::shared_ptr<ScriptType> getType() const override;

	private:
		std::unique_ptr<ScriptStatement> condition = nullptr;
		std::unique_ptr<ScriptIf> selse = nullptr;
	};
}

#endif