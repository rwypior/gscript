#ifndef _h_gscript_scope
#define _h_gscript_scope

#include "variable.hpp"
#include "defs.hpp"
#include "lib.hpp"

#include <string>
#include <list>
#include <vector>
#include <memory>

namespace gscript
{
	class ScriptFunction;
	class ScriptVariable;
	class ScriptStatement;
	class ScriptNamespace;
	class EntityPath;
	class ScriptCallable;

	class ScriptScopeBase
	{
	public:
		ScriptScopeBase() = default;
		ScriptScopeBase(const ScriptScopeBase& scope) = delete;
		virtual ~ScriptScopeBase() = default;

		SCRIPT_API virtual ScriptFunction& registerFunction(const std::string& name,
			std::shared_ptr<ScriptType> returnType,
			const PARAMS_T& parameters = PARAMS_T(),
			std::vector<std::shared_ptr<ScriptCallable>>&& statements = {});
		SCRIPT_API virtual ScriptFunction& registerFunction(std::unique_ptr<ScriptFunction>&& function);

		SCRIPT_API virtual ScriptVariable& registerVariable(const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptValue>&& value);
		SCRIPT_API virtual ScriptVariable& registerVariable(const std::string& name, const std::shared_ptr<ScriptType> type, const std::unique_ptr<ScriptValue>& value);
		SCRIPT_API virtual ScriptVariable& registerVariable(std::unique_ptr<ScriptVariable>&& variable);

		SCRIPT_API virtual ScriptFunction* findFunction(const std::string& name, const PARAMS_T params, bool searchParents = true, bool searchBase = true) const;
		SCRIPT_API virtual ScopedAddress findFunctionAddr(const std::string& name, const PARAMS_T params, bool searchParents = true, bool searchBase = true);
		SCRIPT_API virtual ScriptFunction* getFunction(const std::string& name, const PARAMS_T params, bool searchParents = true, bool searchBase = true) const;
		SCRIPT_API virtual std::vector<std::unique_ptr<ScriptFunction>>& getFunctions() = 0;
		SCRIPT_API virtual const std::vector<std::unique_ptr<ScriptFunction>>& getFunctions() const = 0;

		SCRIPT_API virtual ScriptVariable* findVariable(const std::string& name);
		SCRIPT_API virtual ScopedAddress findVariableAddr(const std::string& name, bool searchParents = true);
		SCRIPT_API virtual ScriptVariable* getVariable(const std::string& name);
		SCRIPT_API virtual std::vector<std::unique_ptr<ScriptVariable>>& getVariables() = 0;
		SCRIPT_API virtual const std::vector<std::unique_ptr<ScriptVariable>>& getVariables() const = 0;

		SCRIPT_API ScriptNamespace* getClosestNamespace(bool includeSelf = false);
		SCRIPT_API ScriptNamespace* getGlobalNamespace();

		SCRIPT_API virtual void setParentScope(ScriptScopeBase* scope) = 0;
		SCRIPT_API virtual ScriptScopeBase* getParentScope() const = 0;
		SCRIPT_API virtual bool isParentOf(ScriptScopeBase& scope, bool includeGrandparents = true) const;
		SCRIPT_API virtual bool isBaseOf(const ScriptScopeBase&) const { return false; }

		SCRIPT_API bool isAccessible(const ScriptScopeBase& targetScope, Modifier access) const;
	};

	class ScriptScope : public ScriptScopeBase
	{
	public:
		SCRIPT_API ScriptScope(const ScriptScope& b);
		SCRIPT_API ScriptScope(ScriptScopeBase* parentScope);
		//ScriptScope(const ScriptScope& scope) = delete;

		SCRIPT_API virtual std::vector<std::unique_ptr<ScriptFunction>>& getFunctions() override;
		SCRIPT_API virtual const std::vector<std::unique_ptr<ScriptFunction>>& getFunctions() const override;
		SCRIPT_API virtual std::vector<std::unique_ptr<ScriptVariable>>& getVariables() override;
		SCRIPT_API virtual const std::vector<std::unique_ptr<ScriptVariable>>& getVariables() const override;

		SCRIPT_API void setParentScope(ScriptScopeBase* scope) override;
		SCRIPT_API ScriptScopeBase* getParentScope() const override;

	protected:
		ScriptScopeBase* parentScope = nullptr;

		std::vector<std::unique_ptr<ScriptFunction>> functions;
		std::vector<std::unique_ptr<ScriptVariable>> variables;
	};
}

#endif