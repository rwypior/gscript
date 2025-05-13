#ifndef _h_script_globalnamespace
#define _h_script_globalnamespace

#include "namespace.hpp"

#include "lib.hpp"

#include <unordered_map>
#include <string>

namespace gscript
{
	class ScriptExternFunction;

	class ScriptGlobalNamespace : public ScriptNamespace
	{
	protected:
		Script &script;

	public:
		typedef std::unordered_map<std::string, ScriptExternFunction*> EXTERN_FUNCTION_CONTAINER;

		ScriptGlobalNamespace::EXTERN_FUNCTION_CONTAINER externFunctions;

		SCRIPT_API ScriptGlobalNamespace(Script &script);

		SCRIPT_API void registerExternFunction(ScriptExternFunction *func);
		SCRIPT_API ScriptExternFunction *findExternFunction(const std::string &name);

		SCRIPT_API Script &getScript();
	};
}

#endif