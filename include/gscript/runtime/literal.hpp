#ifndef _h_gscript_literal
#define _h_gscript_literal

#include "callable.hpp"
#include "scriptValue.hpp"

#include <memory>

namespace gscript
{
	class ScriptLiteral : public ScriptCallable
	{
	public:
		std::unique_ptr<ScriptValue> val = nullptr;

		SCRIPT_API ScriptLiteral(const ScriptLiteral& literal);
		SCRIPT_API ScriptLiteral(std::unique_ptr<ScriptValue> &&val = nullptr);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;
	};
}

#endif