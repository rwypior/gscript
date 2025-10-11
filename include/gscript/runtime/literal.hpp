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

		ScriptLiteral(ScriptScopeBase& scope, std::unique_ptr<ScriptValue> &&val = nullptr);
		//ScriptLiteral(ScriptScope &scope, ParserLiteral pliteral);
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		virtual const std::shared_ptr<ScriptType> getType() const override;
	};
}

#endif