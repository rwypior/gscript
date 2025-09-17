#include "defs.hpp"
#include "runtime/scriptValue.hpp"

namespace gscript
{
	std::shared_ptr<ScriptNullValue> script_null;
	std::shared_ptr<ScriptBoolValue> script_true = std::make_shared<ScriptBoolValue>(true);
	std::shared_ptr<ScriptBoolValue> script_false = std::make_shared<ScriptBoolValue>(false);

	/*ScriptNullValue* SCR_NULL = new ScriptNullValue;
	ScriptBoolValue* SCR_TRUE = new ScriptBoolValue(true);
	ScriptBoolValue* SCR_FALSE = new ScriptBoolValue(false);*/

	/*const char *KW_NULL = "NULL";
	const char *KW_TRUE = "TRUE";
	const char *KW_FALSE = "FALSE";*/

	const std::shared_ptr<ScriptNullValue> getNull()
	{
		return script_null;
	}

	const std::shared_ptr<ScriptBoolValue> getTrue()
	{
		return script_true;
	}

	const std::shared_ptr<ScriptBoolValue> getFalse()
	{
		return script_false;
	}
}