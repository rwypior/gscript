#include "runtime/globalNamespace.hpp"
#include "runtime/function.hpp"
#include "runtime/class.hpp"

namespace gscript
{
	ScriptGlobalNamespace::ScriptGlobalNamespace(Script &script)
		:script(script)
	{ }

	void ScriptGlobalNamespace::registerExternFunction(ScriptExternFunction *func)
	{
		this->externFunctions.emplace(std::make_pair(func->getName(), func));
	}

	ScriptExternFunction *ScriptGlobalNamespace::findExternFunction(const std::string &name)
	{
		EXTERN_FUNCTION_CONTAINER::const_iterator it = this->externFunctions.find(name);
		if (it != this->externFunctions.end())
			return it->second;

		return NULL;
	}

	Script &ScriptGlobalNamespace::getScript()
	{
		return this->script;
	}
}