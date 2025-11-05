#ifndef _h_gscript_executiveblock
#define _h_gscript_executiveblock

#include "gscript/lib.hpp"
#include "gscript/runtime/callable.hpp"

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
		SCRIPT_API ScriptExecutiveBlock(std::vector<std::unique_ptr<ScriptCallable>>&& statements = {});
		SCRIPT_API ScriptExecutiveBlock(ScriptExecutiveBlock&& exeblock) noexcept;
		SCRIPT_API ScriptExecutiveBlock(const ScriptExecutiveBlock& exeblock);
		SCRIPT_API virtual ~ScriptExecutiveBlock();
		SCRIPT_API std::unique_ptr<ScriptValue> execute(ScriptScopeBase& scope);

		SCRIPT_API void merge(std::vector<std::unique_ptr<ScriptCallable>>&& statements);
		SCRIPT_API void merge(ScriptExecutiveBlock&& block);
		SCRIPT_API void merge(std::unique_ptr<ScriptExecutiveBlock>&& block);

		SCRIPT_API std::vector<std::unique_ptr<ScriptCallable>> &getStatements();

	protected:
		std::vector<std::unique_ptr<ScriptCallable>> statements;
	};
}

#endif