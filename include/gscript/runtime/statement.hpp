#ifndef _h_gscript_statement
#define _h_gscript_statement

#include "gscript/lib.hpp"
#include "gscript/runtime/callable.hpp"

#include <vector>
#include <memory>

namespace gscript
{
	class ScriptValue;
	class ScriptOperator;
	class ScriptCallable;
	class ScriptMethod;

	class ScriptStatement : public ScriptCallable
	{
	public:
		std::shared_ptr<ScriptCallable> callable = nullptr;

		SCRIPT_API ScriptStatement(ScriptStatement&& stmt) noexcept;
		SCRIPT_API ScriptStatement(const ScriptStatement& b);
		SCRIPT_API ScriptStatement(std::vector<std::unique_ptr<ScriptCallable>>&& callables);
		SCRIPT_API ScriptStatement(std::unique_ptr<ScriptCallable>&& callable);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual void setup(ScriptScopeBase& scope) override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;

	protected:
		// count of processed callables
		SCRIPT_API int resolveOperations(
			std::vector<std::shared_ptr<ScriptCallable>>::reverse_iterator begin,
			std::vector<std::shared_ptr<ScriptCallable>>::reverse_iterator end,
			std::shared_ptr<ScriptCallable>& result,
			unsigned int depth = 0
		);

		SCRIPT_API void assertAccessibilityOf(const ScriptScopeBase& scope, ScriptMethod& method) const;
		SCRIPT_API void assignReferences(std::shared_ptr<ScriptCallable>& entry, ScriptScopeBase& scope, bool member);
		SCRIPT_API void setupOperator(std::shared_ptr<ScriptOperator> oper);
	};
}

#endif