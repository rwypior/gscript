#ifndef _h_gscript_scope
#define _h_gscript_scope

#include <vector>
#include <string>
#include <list>

#include "variable.hpp"
#include "defs.hpp"

#include "lib.hpp"

namespace gscript
{
	class ScriptFunction;
	class ParserFunction;
	class ScriptStatement;
	class ScriptNamespace;
	class ScriptGlobalNamespace;
	class EntityPath;
	class ParserVarDeclaration;
	class ScriptFunctionPrototype;
	class ScriptExternFunction;

	class ScriptScope
	{
	public:
		typedef std::list<ScriptFunction*> FUNCTION_CONTAINER;
		typedef std::list<ScriptFunctionPrototype*> FUNCTION_PROTOTYPE_CONTAINER;
		typedef std::list<ScriptVariable> VARIABLE_CONTAINER;

	public:
		SCRIPT_API ScriptScope(ScriptScope *parentScope);

		SCRIPT_API virtual void registerFunction(ScriptFunction *c);
		SCRIPT_API virtual ScriptFunction &registerFunction(const ParserFunction &c);
		SCRIPT_API virtual ScriptFunction &registerFunctionPrototype(const ParserFunction &c);

		SCRIPT_API virtual ScriptVariable &registerVariable(const ParserVarDeclaration &pvar, ScriptValue *value);
		SCRIPT_API virtual ScriptVariable &registerVariable(const std::string &name, const ScriptType *type, ScriptValue *value);

		SCRIPT_API virtual ScriptFunction *findFunction(const std::string &name, const PARAMS_T params) const;
		SCRIPT_API virtual ScriptFunction *getFunction(const std::string &name, const PARAMS_T params) const;

		SCRIPT_API virtual ScriptVariable *findVariable(const std::string &name);
		SCRIPT_API virtual ScriptVariable *getVariable(const std::string &name);

		SCRIPT_API ScriptScope::VARIABLE_CONTAINER &getVariables();

		SCRIPT_API ScriptNamespace *getClosestNamespace(bool includeSelf = false);
		SCRIPT_API ScriptGlobalNamespace *getGlobalNamespace();

		SCRIPT_API ScriptExternFunction &getExternFunction(const std::string &name);

		SCRIPT_API void setParentScope(ScriptScope *scope);
		SCRIPT_API ScriptScope *getParentScope() const;

		SCRIPT_API bool isAccessible(ScriptScope &targetScope, BITFLAG_T access);

	protected:
		ScriptScope *parentScope = nullptr;

		ScriptScope::FUNCTION_CONTAINER functions;
		ScriptScope::FUNCTION_PROTOTYPE_CONTAINER functionPrototypes;
		ScriptScope::VARIABLE_CONTAINER variables;
	};
}

#endif