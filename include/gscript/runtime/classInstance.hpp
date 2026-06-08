#ifndef _h_gscript_classinstance
#define _h_gscript_classinstance

#include "gscript/lib.hpp"
#include "gscript/runtime/scope.hpp"

namespace gscript
{
	class ScriptClass;
	class ScriptMethod;

	class ScriptClassInstance : public ScriptScopeBase
	{
	public:
		using VariableContainer = std::vector<std::unique_ptr<ScriptVariable>>;

	public:
		SCRIPT_API virtual ~ScriptClassInstance();
		SCRIPT_API ScriptClassInstance(const ScriptClassInstance& sclass);
		SCRIPT_API ScriptClassInstance(ScriptClass& sclass);

		SCRIPT_API ScriptClass &getClass();

		SCRIPT_API virtual std::vector<std::unique_ptr<ScriptFunction>>& getFunctions() override;
		SCRIPT_API virtual const std::vector<std::unique_ptr<ScriptFunction>>& getFunctions() const;
		SCRIPT_API virtual std::vector<std::unique_ptr<ScriptVariable>>& getVariables() override;
		SCRIPT_API virtual const std::vector<std::unique_ptr<ScriptVariable>>& getVariables() const;

		SCRIPT_API virtual ScriptMethod* findMethod(const std::string& name, const PARAMS_T& params, bool searchParents = true, bool searchBase = true) const;
		SCRIPT_API virtual ScriptMethod* findOperator(OperatorType operatorType, const PARAMS_T& params, bool searchParents = true, bool searchBase = true) const;

		SCRIPT_API void setParentScope(ScriptScopeBase* scope) override;
		SCRIPT_API ScriptScope* getParentScope() const override;

	protected:
		ScriptClass &sclass;
		std::vector<std::unique_ptr<ScriptVariable>> variables;
	};
}

#endif