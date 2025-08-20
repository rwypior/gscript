#include "runtime/classInstance.hpp"
#include "runtime/class.hpp"

#include <algorithm>

namespace gscript
{
	ScriptClassInstance::ScriptClassInstance(ScriptClass &sclass)
		: sclass(sclass)
		//, variables(sclass.getVariables().begin(), sclass.getVariables().end())
	{
		std::transform(sclass.getVariables().begin(), sclass.getVariables().end(), this->variables.begin(), [](std::unique_ptr<ScriptVariable>& v) {
			return std::make_unique<ScriptVariable>(*v);
		});
	}

	ScriptClass &ScriptClassInstance::getClass()
	{
		return this->sclass;
	}

	std::vector<std::unique_ptr<ScriptVariable>>&ScriptClassInstance::getVariables()
	{
		return this->variables;
	}
}