#ifndef _h_gscript_array_initializer
#define _h_gscript_array_initializer

#include "defs.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "funcCall.hpp"
#include "lib.hpp"
#include "type.hpp"
#include "scope.hpp"

#include <memory>

namespace gscript
{
	class ScriptArrayInitializer : public ScriptCallable
	{
	public:
		SCRIPT_API ScriptArrayInitializer(ScriptScope &scope, std::vector<std::unique_ptr<ScriptStatement>>&& statements);

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const;

	protected:
		std::shared_ptr<ScriptType> type;
		std::vector<std::unique_ptr<ScriptStatement>> statements;
	};
}

#endif