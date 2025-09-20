#ifndef _h_gscript_method
#define _h_gscript_method

#include "function.hpp"
#include "defs.hpp"
#include "lib.hpp"

#include <string>

namespace gscript
{
	class ScriptScope;

	class ScriptMethod : public ScriptFunction
	{
	public:
		const Modifier accessModifier = Modifier::None;

		SCRIPT_API ScriptMethod(
			ScriptScopeBase& scope,
			const std::string &name,
			std::shared_ptr<ScriptType> returnType,
			const PARAMS_T &parameters = PARAMS_T(),
			Modifier modifier = Modifier::None
		);
		
		SCRIPT_API bool isStatic() const;
		SCRIPT_API bool isVirtual() const;
		SCRIPT_API bool isAbstract() const;

		SCRIPT_API void setClassInstance(std::unique_ptr<ScriptClassInstance>&& inst);
		SCRIPT_API void setClassInstance(std::unique_ptr<ScriptValue>&& inst);
		SCRIPT_API ScriptClassInstance *getClassInstance();

		// Run this method from the class instance
		SCRIPT_API virtual std::unique_ptr<ScriptValue> instrun(std::unique_ptr<ScriptReferenceValue>&& instance, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

	protected:
		void createThis(ScriptClass &sclass);
	};
}

#endif