#ifndef _h_gscript_namespace
#define _h_gscript_namespace

#include "scope.hpp"
#include "lib.hpp"

#include <string>
#include <vector>
#include <memory>

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
		ScriptNamespace(const ScriptNamespace&) = delete;
		SCRIPT_API ScriptNamespace(ScriptScopeBase* parentScope = NULL);
		SCRIPT_API ScriptNamespace(ScriptScopeBase* parentScope, const std::string &name);
		
		//SCRIPT_API ScriptClass &registerClassPrototype(const ParserClass &c);
		//SCRIPT_API ScriptClass &registerClass(const ParserClass &c);
		SCRIPT_API ScriptClass& registerClass(std::unique_ptr<ScriptClass> &&c);

		//SCRIPT_API ScriptNamespace &registerNamespace(const ParserNamespace &ns);
		//SCRIPT_API ScriptNamespace &registerNamespace(ScriptNamespace *ns);
		SCRIPT_API ScriptNamespace &registerNamespace(std::unique_ptr<ScriptNamespace> &&ns);

		SCRIPT_API ScriptClass *findClass(const std::string &name);
		SCRIPT_API ScriptClass *findClassByInheritance(const std::string &name);
		SCRIPT_API std::vector<std::unique_ptr<ScriptClass>>& getClasses();

		SCRIPT_API ScriptNamespace *findNamespace(const std::string &name);
		SCRIPT_API std::vector<std::unique_ptr<ScriptNamespace>>& getNamespaces();

		SCRIPT_API const std::string &getName() const;

		void resolveFunctions();
		//void resolveClasses();
		void resolveClassMembers();

	protected:
		//NAMESPACE_CONTAINER_T namespaces;
		std::vector<std::unique_ptr<ScriptNamespace>> namespaces;
		std::vector<std::unique_ptr<ScriptClass>> classes;
		//std::unordered_map<std::string, std::unique_ptr<ScriptClassPrototype>> classPrototypes;
		//CLASS_PROTOTYPE_CONTAINER_T classPrototypes;

	private:
		std::string name = "";
	};
}

#endif