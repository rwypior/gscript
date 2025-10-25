#ifndef _h_gscript_statement
#define _h_gscript_statement

#include "callable.hpp"
#include "lib.hpp"

#include <vector>
#include <memory>

namespace gscript
{
	//class ParserStatement;
	class ScriptValue;
	class ScriptOperator;
	class ScriptCallable;
	class ScriptMethod;

	class ScriptStatement : public ScriptCallable
	{
	public:
		//ScriptCallable *callable = nullptr;
		std::shared_ptr<ScriptCallable> callable = nullptr;

		//SCRIPT_API ScriptStatement(ScriptScope &scope, const ParserStatement &pstatement);
		SCRIPT_API ScriptStatement(ScriptStatement&& stmt) noexcept;
		SCRIPT_API ScriptStatement(const ScriptStatement& b);
		SCRIPT_API ScriptStatement(ScriptScope &scope);
		SCRIPT_API ScriptStatement(ScriptScope &scope, std::vector<std::unique_ptr<ScriptCallable>>&& callables);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual void setup(ScriptScopeBase& scope) override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;

	protected:
		/// <return>count of processed callables</return>
		SCRIPT_API int resolveOperations(
			ScriptScopeBase& scope, 
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