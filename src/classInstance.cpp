#include "classInstance.hpp"
#include "class.hpp"

namespace gscript
{
	ScriptClassInstance::ScriptClassInstance(ScriptClass &sclass)
		:sclass(sclass),
		variables(sclass.getVariables().begin(), sclass.getVariables().end())
	{
	}

	ScriptClass &ScriptClassInstance::getClass()
	{
		return this->sclass;
	}

	ScriptClassInstance::INSTANCE_VARIABLES_CONTAINER_T &ScriptClassInstance::getVariables()
	{
		return this->variables;
	}
}