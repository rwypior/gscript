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

		ScriptLiteral(ScriptScope &scope, std::unique_ptr<ScriptValue> &&val = nullptr);
		//ScriptLiteral(ScriptScope &scope, ParserLiteral pliteral);
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		virtual const ScriptType *getType() const override;
	};
}

#endif