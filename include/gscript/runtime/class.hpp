#ifndef _h_gscript_class
#define _h_gscript_class

#include "defs.hpp"
#include "method.hpp"
#include "scope.hpp"
#include "namespace.hpp"
#include "lib.hpp"

#include <string>
#include <vector>

namespace gscript
{
	class ScriptMethod;
	class ParserMethod;
	class ScriptFieldDeclaration;
	class ParserClass;
	class ScriptClassPrototype;
	class ScriptClassInstance;

	class ScriptClass : public ScriptNamespace
	{
	public:
		static const char *KW_THIS;

		//typedef std::vector<ScriptVarDeclaration*> VAR_DECLARATION_CONTAINER;

	public:
		SCRIPT_API ScriptClass(ScriptScopeBase& scope, const std::string &name, ScriptClass *base = nullptr);

		SCRIPT_API ScriptMethod *findMethod(const std::string &name, const PARAMS_T &params);
		SCRIPT_API ScriptClass *getBase() const;

		SCRIPT_API bool isBaseOf(ScriptClass *base);

		//SCRIPT_API virtual void registerMethodPrototype(const ParserMethod &m);
		SCRIPT_API virtual ScriptFunction& registerFunction(std::unique_ptr<ScriptFunction>&& f);
		//SCRIPT_API virtual ScriptFunction &registerFunction(const ParserFunction &c);

		SCRIPT_API std::vector<ScriptMethod*> getAbstractMethods();

		ScriptMethod *getConstructor()
		{
			return this->constructor;
		}

		SCRIPT_API ScriptClassInstance *instantiate(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		SCRIPT_API void initialize(ScriptClassInstance &instance);
		SCRIPT_API void addFieldDeclaration(ScriptFieldDeclaration *svd);

		SCRIPT_API const std::string &getName() const;

		SCRIPT_API bool operator ==(const ScriptClass &b) const;

		SCRIPT_API void makeAbstract();
		SCRIPT_API bool isAbstract();

		SCRIPT_API void setup();

	protected:
		const std::string name;
		ScriptClass* base = nullptr;
		ScriptMethod* constructor = nullptr;

		//ScriptClass::VAR_DECLARATION_CONTAINER varDeclarations;
		std::vector<ScriptFieldDeclaration*> fieldDeclarations;

		CLASS_MODIFIER_T modifier = CLASS_MODIFIER_T::CM_NONE;

		SCRIPT_API void assignConstructor(ScriptFunction& f);
		void createThis();

		void inheritAbstracts();
	};

	//class ScriptClassResolv : public ScriptClass
	//{
	//public:
	//	SCRIPT_API ScriptClassResolv(ScriptNamespace *snamespace, ParserClass *pClass);

	//	ScriptClass *resolve();

	//protected:
	//	ScriptNamespace *snamespace = nullptr;
	//	ParserClass *pClass = nullptr;

	//};

	//class ScriptClassPrototype
	//{
	//public:
	//	SCRIPT_API ScriptClassPrototype(ScriptClass &target, const ParserClass &pClass);

	//	void build();

	//protected:
	//	ScriptClass &target;
	//	const ParserClass &pClass;
	//};
}

#endif