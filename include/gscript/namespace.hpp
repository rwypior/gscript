#ifndef _h_gscript_namespace
#define _h_gscript_namespace

#include "scope.hpp"

#include "lib.hpp"

#include <string>

namespace gscript
{
	class ParserClass;
	class ParserNamespace;
	class Script;
	class ScriptClass;
	class ScriptClassPrototype;

	class ScriptNamespace : public ScriptScope
	{
	public:
		typedef std::list<ScriptNamespace*> NAMESPACE_CONTAINER_T;
		typedef std::list<ScriptClassPrototype*> CLASS_PROTOTYPE_CONTAINER_T;
		typedef std::list<ScriptClass*> CLASS_CONTAINER_T;
				
	public:
		SCRIPT_API ScriptNamespace(ScriptScope *parentScope = NULL);
		SCRIPT_API ScriptNamespace(ScriptScope *parentScope, const ParserNamespace &ns);
		SCRIPT_API ScriptNamespace(ScriptScope *parentScope, const std::string &name);
		
		SCRIPT_API ScriptClass &registerClassPrototype(const ParserClass &c);
		SCRIPT_API void registerClass(ScriptClass *c);
		SCRIPT_API ScriptClass &registerClass(const ParserClass &c);

		SCRIPT_API ScriptNamespace &registerNamespace(const ParserNamespace &ns);
		SCRIPT_API ScriptNamespace &registerNamespace(ScriptNamespace *ns);

		SCRIPT_API ScriptClass *findClass(const std::string &name);
		SCRIPT_API ScriptClass *findClassByInheritance(const std::string &name);

		SCRIPT_API ScriptNamespace *findNamespace(const std::string &name);

		SCRIPT_API const std::string &getName() const;

		void resolveFunctions();
		void resolveClasses();
		void resolveClassMembers();

	protected:
		NAMESPACE_CONTAINER_T namespaces;
		CLASS_CONTAINER_T classes;
		CLASS_PROTOTYPE_CONTAINER_T classPrototypes;

	private:
		std::string name = "";
	};
}

#endif