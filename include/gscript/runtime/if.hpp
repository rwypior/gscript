#ifndef _h_gscript_if
#define _h_gscript_if

#include "variable.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "scope.hpp"
#include "executiveBlock.hpp"
#include "lib.hpp"

#include <vector>
#include <memory>

namespace gscript
{
	class ScriptIf : public ScriptScope, public ScriptExecutiveBlock, public ScriptCallable
	{
	public:
		SCRIPT_API ScriptIf(const ScriptIf& sif);
		SCRIPT_API ScriptIf(ScriptScope& scope, std::unique_ptr<ScriptStatement>&& condition, std::unique_ptr<ScriptIf>&& selse = {}, std::vector<std::unique_ptr<ScriptCallable>>&& statements = {});

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;

		SCRIPT_API const std::unique_ptr<ScriptCallable>& getCondition() const;
		SCRIPT_API std::unique_ptr<ScriptCallable>& getCondition();

	private:
		std::unique_ptr<ScriptCallable> condition = nullptr;
		std::unique_ptr<ScriptIf> selse = nullptr;
	};
}

#endif