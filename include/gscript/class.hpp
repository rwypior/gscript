#ifndef _h_script_class
#define _h_script_class

#include <string>
#include <vector>

#include "defs.hpp"
#include "method.hpp"
#include "scope.hpp"
#include "namespace.hpp"

#include "lib.hpp"

namespace gscript
{
	class ScriptMethod;
	class ParserMethod;
	class ScriptVarDeclaration;
	class ParserClass;
	class ScriptClassPrototype;
	class ScriptClassInstance;

	class ScriptClass : public ScriptNamespace
	{
	public:
		static const char *KW_THIS;

		typedef std::vector<ScriptVarDeclaration*> VAR_DECLARATION_CONTAINER;

	protected:
		const std::string name;
		ScriptClass * base;
		ScriptMethod *constructor = NULL;

		ScriptClass::VAR_DECLARATION_CONTAINER varDeclarations;

		BITFLAG_T modifier;

		SCRIPT_API void assignConstructor(ScriptFunction &f);
		void createThis();

		void inheritAbstracts();

	public:
		SCRIPT_API ScriptClass(ScriptScope &scope, const std::string &name, ScriptClass *base = NULL);

		SCRIPT_API ScriptMethod *findMethod(const std::string &name, const PARAMS_T &params);
		SCRIPT_API ScriptClass *getBase() const;

		SCRIPT_API bool isBaseOf(ScriptClass *base);

		SCRIPT_API virtual void registerMethodPrototype(const ParserMethod &m);
		SCRIPT_API virtual void registerFunction(ScriptFunction *f);
		SCRIPT_API virtual ScriptFunction &registerFunction(const ParserFunction &c);

		SCRIPT_API std::vector<ScriptMethod*> getAbstractMethods();

		ScriptMethod *getConstructor()
		{
			return this->constructor;
		}

		SCRIPT_API ScriptClassInstance *instantiate(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		SCRIPT_API void initialize(ScriptClassInstance &instance);
		SCRIPT_API void addVarDeclaration(ScriptVarDeclaration *svd);

		SCRIPT_API const std::string &getName() const;

		SCRIPT_API bool operator ==(const ScriptClass &b) const;

		SCRIPT_API void makeAbstract();
		SCRIPT_API bool isAbstract();

		SCRIPT_API void setup();
	};

	class ScriptClassResolv : public ScriptClass
	{
	protected:
		ScriptNamespace *snamespace;
		ParserClass *pClass;

	public:
		SCRIPT_API ScriptClassResolv(ScriptNamespace *snamespace, ParserClass *pClass);

		ScriptClass *resolve();
	};

	class ScriptClassPrototype
	{
	protected:
		ScriptClass &target;
		const ParserClass &pClass;

	public:
		SCRIPT_API ScriptClassPrototype(ScriptClass &target, const ParserClass &pClass);

		void build();
	};
}

#endif