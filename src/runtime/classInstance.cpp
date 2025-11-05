#include "gscript/runtime/classInstance.hpp"
#include "gscript/runtime/class.hpp"

#include <algorithm>

namespace gscript
{
	ScriptClassInstance::ScriptClassInstance(const ScriptClassInstance& b)
		: sclass(b.sclass)
	{
		for (auto& el : b.variables)
		{
			this->variables.push_back(std::make_unique<ScriptVariable>(*el));
		}
	}

	ScriptClassInstance::ScriptClassInstance(ScriptClass &sclass)
		: sclass(sclass)
	{
		this->variables.resize(sclass.getVariables().size());
		std::transform(sclass.getVariables().begin(), sclass.getVariables().end(), this->variables.begin(), [](std::unique_ptr<ScriptVariable>& v) {
			return std::make_unique<ScriptVariable>(*v);
		});
	}

	ScriptClass &ScriptClassInstance::getClass()
	{
		return this->sclass;
	}

	std::vector<std::unique_ptr<ScriptFunction>>& ScriptClassInstance::getFunctions()
	{
		return this->sclass.getFunctions();
	}

	const std::vector<std::unique_ptr<ScriptFunction>>& ScriptClassInstance::getFunctions() const
	{
		return this->sclass.getFunctions();
	}

	std::vector<std::unique_ptr<ScriptVariable>>& ScriptClassInstance::getVariables()
	{
		return this->variables;
	}

	const std::vector<std::unique_ptr<ScriptVariable>>& ScriptClassInstance::getVariables() const
	{
		return this->variables;
	}

	ScriptMethod* ScriptClassInstance::findMethod(const std::string& name, const PARAMS_T params, bool searchParents, bool searchBase) const
	{
		return this->sclass.findMethod(name, params, searchParents, searchBase);
	}

	void ScriptClassInstance::setParentScope(ScriptScopeBase*)
	{
		throw RuntimeException("Cannot set parent scope on class instance");
	}

	ScriptScope* ScriptClassInstance::getParentScope() const
	{
		return &this->sclass;
	}
}