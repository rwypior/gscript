#ifndef _h_gscript_classinstance
#define _h_gscript_classinstance

#include "scope.hpp"

#include "lib.hpp"

namespace gscript
{
	class ScriptClass;

	class ScriptClassInstance
	{
	public:
		typedef std::vector<ScriptVariable> INSTANCE_VARIABLES_CONTAINER_T;

	protected:
		ScriptClass & sclass;
		//ScriptScope::VARIABLE_CONTAINER variables;
		INSTANCE_VARIABLES_CONTAINER_T variables;

	public:
		SCRIPT_API ScriptClassInstance(ScriptClass &sclass);

		SCRIPT_API ScriptClass &getClass();
		SCRIPT_API INSTANCE_VARIABLES_CONTAINER_T &getVariables();
	};
}

#endif