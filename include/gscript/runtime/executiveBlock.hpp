#ifndef _h_gscript_executiveblock
#define _h_gscript_executiveblock

#include "lib.hpp"
#include "callable.hpp"

#include <vector>
#include <memory>

namespace gscript
{
	class ScriptValue;

	// ScriptExecutiveBlock is a single block of executable statements - anything that
	// can be run as a code - typically a part of another script construct

	class ScriptExecutiveBlock
	{
	public:
		SCRIPT_API ScriptExecutiveBlock(std::vector<std::shared_ptr<ScriptCallable>>&& statements = {});
		SCRIPT_API ScriptExecutiveBlock(ScriptExecutiveBlock&& exeblock) noexcept;
		SCRIPT_API virtual ~ScriptExecutiveBlock() = default;
		SCRIPT_API std::unique_ptr<ScriptValue> execute(ScriptScopeBase& scope);

		SCRIPT_API void merge(ScriptExecutiveBlock&& block);
		SCRIPT_API void merge(std::unique_ptr<ScriptExecutiveBlock>&& block);

		SCRIPT_API std::vector<std::shared_ptr<ScriptCallable>> &getStatements();

	protected:
		std::vector<std::shared_ptr<ScriptCallable>> statements;
	};
}

#endif