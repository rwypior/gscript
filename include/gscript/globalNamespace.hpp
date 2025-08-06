#ifndef _h_gscript_globalnamespace
#define _h_gscript_globalnamespace

#include "namespace.hpp"

#include "lib.hpp"

#include <unordered_map>
#include <string>

namespace gscript
{
	class ScriptExternFunction;

	class ScriptGlobalNamespace : public ScriptNamespace
	{
	public:
		typedef std::unordered_map<std::string, ScriptExternFunction*> EXTERN_FUNCTION_CONTAINER;

		ScriptGlobalNamespace::EXTERN_FUNCTION_CONTAINER externFunctions;

		SCRIPT_API ScriptGlobalNamespace(Script &script);

		SCRIPT_API void registerExternFunction(ScriptExternFunction *func);
		SCRIPT_API ScriptExternFunction *findExternFunction(const std::string &name);

		SCRIPT_API Script &getScript();

	protected:
		Script &script;
	};
}

#endif