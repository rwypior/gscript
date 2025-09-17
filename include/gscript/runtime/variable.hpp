#ifndef _h_gscript_variable
#define _h_gscript_variable

#include "defs.hpp"
#include "lib.hpp"

#include <string>

namespace gscript
{
	class ScriptType;
	class ScriptValue;

	class ScriptVariable
	{
	public:
		SCRIPT_API ScriptVariable(const std::string &name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptValue>&& value);
		SCRIPT_API ScriptVariable(const ScriptVariable& b);
		SCRIPT_API virtual ~ScriptVariable() = default;

		SCRIPT_API void init(std::unique_ptr<ScriptValue>&& value);

		SCRIPT_API const std::unique_ptr<ScriptValue>& getValue() const;
		SCRIPT_API void setValue(std::unique_ptr<ScriptValue>&& value);
		SCRIPT_API void setValue(const std::unique_ptr<ScriptValue>& value);

		SCRIPT_API const std::string& getName() const;
		SCRIPT_API const std::shared_ptr<ScriptType> getType() const;

	protected:
		const std::string name;
		std::unique_ptr<ScriptValue> value;
		const std::shared_ptr<ScriptType> type;
	};
}

#endif