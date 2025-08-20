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
		using VariableContainer = std::vector<std::unique_ptr<ScriptVariable>>;

	public:
		SCRIPT_API ScriptClassInstance(ScriptClass &sclass);

		SCRIPT_API ScriptClass &getClass();
		SCRIPT_API std::vector<std::unique_ptr<ScriptVariable>>& getVariables();

	protected:
		ScriptClass &sclass;
		std::vector<std::unique_ptr<ScriptVariable>> variables;
	};
}

#endif