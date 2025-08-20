#include "defs.hpp"
#include "runtime/scriptValue.hpp"

namespace gscript
{
	ScriptNullValue* SCR_NULL = new ScriptNullValue;
	ScriptBoolValue* SCR_TRUE = new ScriptBoolValue(true);
	ScriptBoolValue* SCR_FALSE = new ScriptBoolValue(false);

	const char *KW_NULL = "NULL";
	const char *KW_TRUE = "TRUE";
	const char *KW_FALSE = "FALSE";
}