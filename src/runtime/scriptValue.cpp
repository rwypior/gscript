#include "runtime/scriptValue.hpp"
#include "runtime/classInstance.hpp"

namespace gscript
{
	ScriptClassValue::ScriptClassValue(ScriptClassInstance *v)
		:val(v),
		type(new ScriptClassType(v->getClass()))
	{
	}

	ScriptClassValue::ScriptClassValue(ScriptClassInstance *v, ScriptClass &sclass)
		: val(v),
		type(new ScriptClassType(sclass))
	{
	}
}