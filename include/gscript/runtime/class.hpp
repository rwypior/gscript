#ifndef _h_gscript_class
#define _h_gscript_class

#include "gscript/defs.hpp"
#include "gscript/lib.hpp"
#include "gscript/runtime/method.hpp"
#include "gscript/runtime/scope.hpp"
#include "gscript/runtime/namespace.hpp"
#include "gscript/runtime/varDeclaration.hpp"

#include <string>
#include <vector>
#include <memory>

namespace gscript
{
	class ScriptMethod;
	class ParserMethod;
	class ParserClass;
	class ScriptClassPrototype;
	class ScriptClassInstance;

	class ScriptClass : public ScriptNamespace
	{
	public:
		static constexpr char keywordThis[] = "this";

	public:
		SCRIPT_API ScriptClass(ScriptScopeBase& scope, const std::string &name, ScriptClass *base = nullptr);

		SCRIPT_API ScriptMethod *findMethod(const std::string &name, const PARAMS_T &params, bool searchParents = true, bool searchBase = true);
		SCRIPT_API ScriptClass *getBase() const;

		SCRIPT_API bool isBaseOf(const ScriptClass *base) const;

		SCRIPT_API virtual ScriptFunction& registerFunction(std::unique_ptr<ScriptFunction>&& f);
		SCRIPT_API virtual ScriptMethod* findMethod(const std::string& name, const PARAMS_T params, bool searchParents = true, bool searchBase = true) const;

		SCRIPT_API std::vector<ScriptMethod*> getAbstractMethods();

		ScriptMethod *getConstructor()
		{
			return this->constructor;
		}

		SCRIPT_API std::unique_ptr<ScriptClassInstance> instantiate(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		SCRIPT_API void initialize(ScriptClassInstance &instance);
		SCRIPT_API void addFieldDeclaration(std::unique_ptr<ScriptFieldDeclaration>&& svd);

		SCRIPT_API const std::string &getName() const;

		SCRIPT_API bool operator ==(const ScriptClass &b) const;

		SCRIPT_API void makeAbstract();
		SCRIPT_API bool isAbstract();

		SCRIPT_API void setup();

	protected:
		const std::string name;
		ScriptClass* base = nullptr;
		ScriptMethod* constructor = nullptr;

		std::vector<std::unique_ptr<ScriptFieldDeclaration>> fieldDeclarations;

		ClassModifier modifier = ClassModifier::None;

		SCRIPT_API void assignConstructor(ScriptFunction& f);
		SCRIPT_API void createThis();

		SCRIPT_API void inheritAbstracts();
	};
}

#endif