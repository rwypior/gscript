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

	class ScriptScopeBase
	{
	public:
		ScriptScopeBase() = default;
		ScriptScopeBase(const ScriptScopeBase& scope) = delete;
		virtual ~ScriptScopeBase() = default;

		SCRIPT_API virtual void registerFunction(std::unique_ptr<ScriptFunction>&& function);

		SCRIPT_API virtual ScriptVariable& registerVariable(const std::string& name, const ScriptType* type, ScriptValue* value);
		SCRIPT_API virtual ScriptVariable& registerVariable(std::unique_ptr<ScriptVariable>&& variable);

		SCRIPT_API virtual ScriptFunction* findFunction(const std::string& name, const PARAMS_T params) const;
		SCRIPT_API virtual ScriptFunction* getFunction(const std::string& name, const PARAMS_T params) const;
		SCRIPT_API virtual std::vector<std::unique_ptr<ScriptFunction>>& getFunctions() = 0;
		SCRIPT_API virtual const std::vector<std::unique_ptr<ScriptFunction>>& getFunctions() const = 0;

		SCRIPT_API virtual ScriptVariable* findVariable(const std::string& name);
		SCRIPT_API virtual ScopedAddress findVariableAddr(const std::string& name);
		SCRIPT_API virtual ScriptVariable* getVariable(const std::string& name);
		SCRIPT_API virtual std::vector<std::unique_ptr<ScriptVariable>>& getVariables() = 0;
		SCRIPT_API virtual const std::vector<std::unique_ptr<ScriptVariable>>& getVariables() const = 0;

		SCRIPT_API ScriptNamespace* getClosestNamespace(bool includeSelf = false);
		SCRIPT_API ScriptNamespace* getGlobalNamespace();

		SCRIPT_API virtual void setParentScope(ScriptScopeBase* scope) = 0;
		SCRIPT_API virtual ScriptScopeBase* getParentScope() const = 0;

		SCRIPT_API bool isAccessible(ScriptScopeBase& targetScope, MODIFIER_T access);
	};

	class ScriptScope : public ScriptScopeBase
	{
	public:
		SCRIPT_API ScriptScope(ScriptScope* parentScope);
		ScriptScope(const ScriptScope& scope) = delete;

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



	/*class ScriptScope
	{
	public:
		SCRIPT_API ScriptScope(ScriptScope *parentScope);
		ScriptScope(const ScriptScope& scope) = delete;

		SCRIPT_API virtual void registerFunction(std::unique_ptr<ScriptFunction>&& function);

		SCRIPT_API virtual ScriptVariable& registerVariable(const std::string &name, const ScriptType *type, ScriptValue *value);
		SCRIPT_API virtual ScriptVariable& registerVariable(std::unique_ptr<ScriptVariable>&& variable);

		SCRIPT_API virtual ScriptFunction *findFunction(const std::string &name, const PARAMS_T params) const;
		SCRIPT_API virtual ScriptFunction *getFunction(const std::string &name, const PARAMS_T params) const;
		SCRIPT_API std::vector<std::unique_ptr<ScriptFunction>>& getFunctions();

		SCRIPT_API virtual ScriptVariable *findVariable(const std::string &name);
		SCRIPT_API virtual ScriptVariable *getVariable(const std::string &name);
		SCRIPT_API std::vector<std::unique_ptr<ScriptVariable>> &getVariables();

		SCRIPT_API ScriptNamespace* getClosestNamespace(bool includeSelf = false);
		SCRIPT_API ScriptNamespace* getGlobalNamespace();

		SCRIPT_API void setParentScope(ScriptScope *scope);
		SCRIPT_API ScriptScope *getParentScope() const;

		SCRIPT_API bool isAccessible(ScriptScope &targetScope, MODIFIER_T access);

	protected:
		ScriptScope *parentScope = nullptr;

		std::vector<std::unique_ptr<ScriptFunction>> functions;
		std::vector<std::unique_ptr<ScriptVariable>> variables;
	};*/
}

#endif