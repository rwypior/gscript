#ifndef _h_gscript_statement
#define _h_gscript_statement

#include "callable.hpp"
#include "lib.hpp"

#include <vector>

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
		SCRIPT_API ScriptStatement(ScriptStatement&& stmt);
		ScriptStatement(const ScriptStatement& stmt) = delete;
		SCRIPT_API ScriptStatement(ScriptScope &scope);
		SCRIPT_API ScriptStatement(ScriptScope &scope, std::vector<std::unique_ptr<ScriptCallable>>&& callables);

		SCRIPT_API virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual void setup() override;

		SCRIPT_API virtual const ScriptType *getType() const override;

	protected:
		/// <return>count of processed callables</return>
		SCRIPT_API int resolveOperations(
			std::vector<std::shared_ptr<ScriptCallable>>::reverse_iterator begin,
			std::vector<std::shared_ptr<ScriptCallable>>::reverse_iterator end,
			std::shared_ptr<ScriptCallable>& result,
			unsigned int depth = 0
		);

		SCRIPT_API void assertAccessibilityOf(ScriptMethod& method) const;
		SCRIPT_API void assignReferences(std::shared_ptr<ScriptCallable>& entry, ScriptScopeBase* scope, bool member);
		SCRIPT_API void setupOperator(std::shared_ptr<ScriptOperator> oper);
	};
}

#endif