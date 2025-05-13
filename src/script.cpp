#include <iostream>
#include <fstream>
#include <string>

#include "script.hpp"
#include "IteratorRange.hpp"

#include "pClass.hpp"
#include "pNamespace.hpp"

#include "pStatement.hpp"
#include "pVarDeclaration.hpp"

#include "compileException.hpp"
#include "runtimeException.hpp"

#include "scriptValue.hpp"

#include "classInstance.hpp"
#include "globalNamespace.hpp"

#include "sfPrint.hpp"
#include "sfToString.hpp"

#include "defs.hpp"
#include "type.hpp"
#include "path.hpp"
#include "util.hpp"

#include "scriptExtension.hpp"

#include "ConsoleExtension.hpp"
#include "IOExtension.hpp"
#include "SampleExtension.hpp"

#include "debug.hpp"

namespace gscript
{
	ScriptNullValue *Script::SCR_NULL = new ScriptNullValue;
	ScriptBoolValue *Script::SCR_TRUE = new ScriptBoolValue(true);
	ScriptBoolValue *Script::SCR_FALSE = new ScriptBoolValue(false);

	Script::Script()
		: mainScope(new ScriptGlobalNamespace(*this))
	{
	}

	Script::Script(const std::string &path, const std::string &content)
		:content(content),
		path(path),
		mainScope(new ScriptGlobalNamespace(*this))
	{
	}

	Script::Script(const std::string &path, const std::string &content, ScriptGlobalNamespace *mainScope)
		: content(content),
		path(path),
		mainScope(mainScope),
		isExtern(true)
	{
	}

	Script::Script(const std::string &path, const std::string &content, Script &script)
		: content(content),
		path(path),
		mainScope(script.mainScope),
		extensions(script.extensions),
		isExtern(true)
	{
	}

	Script::~Script()
	{
		if (!this->isExtern)
		{
			if (this->mainScope)
				delete this->mainScope;

			for (EXTENSIONS_CONTAINER_T::iterator it = this->extensions.begin(); it != this->extensions.end(); ++it)
				delete it->second;
		}
	}

	Script &Script::operator=(const Script &b)
	{
		this->path = b.path;
		this->content = b.content;

		return *this;
	}

	void Script::extend(ScriptExtension *extension, const std::string &name)
	{
		this->extensions.emplace(name.empty() ? extension->getName() : name, extension);
	}

	void Script::loadDefaultExtensions()
	{
		this->extend(new ConsoleExtension());
		this->extend(new IOExtension());
		this->extend(new SampleExtension());
	}

	ScriptExtension *Script::findExtension(const std::string &name)
	{
		Script::EXTENSIONS_CONTAINER_T::const_iterator it = this->extensions.find(name);
		if (it == this->extensions.end())
			return nullptr;

		return it->second;
	}

	bool Script::compile()
	{
		ParserNamespace mainNamespace(NAMESPACE_TYPE_T::NT_MAIN);
		mainNamespace.parse(ParserEntity::StringIteratorRange(this->content.begin(), this->content.end()));

		for (ParserNamespace::IMPORT_CONTAINER_T::iterator it = mainNamespace.extensions.begin(); it != mainNamespace.extensions.end(); ++it)
		{
			if (ScriptExtension *ext = this->mainScope->getScript().findExtension(*it))
				ext->load(this->mainScope->getScript());
			else
				throw CompileException(std::string("Extension \"" + *it + "\" could not be found"));
		}

		if (!this->isExtern)
			this->init();
		
		for (ParserNamespace::NAMESPACE_CONTAINER_T::iterator it = mainNamespace.namespaces.begin(); it != mainNamespace.namespaces.end(); ++it)
		{
			this->mainScope->registerNamespace(*it);
		}

		for (ParserNamespace::IMPORT_CONTAINER_T::iterator it = mainNamespace.imports.begin(); it != mainNamespace.imports.end(); ++it)
		{
			this->import(*it);
		}

		for (ParserNamespace::CLASS_CONTAINER_T::iterator it = mainNamespace.classes.begin(); it != mainNamespace.classes.end(); ++it)
		{
			this->mainScope->registerClassPrototype(*it);
		}

		this->mainScope->resolveClasses();

		for (ParserNamespace::FUNCTION_CONTAINER_T::iterator it = mainNamespace.functions.begin(); it != mainNamespace.functions.end(); ++it)
		{
			this->mainScope->registerFunctionPrototype(*it);
		}

		this->mainScope->resolveFunctions();
		this->mainScope->resolveClassMembers();

		return true;
	}

	void Script::init()
	{
		this->mainScope->registerVariable(KW_NULL, Script::SCR_NULL->getType(), Script::SCR_NULL);
		this->mainScope->registerVariable(KW_TRUE, Script::SCR_TRUE->getType(), Script::SCR_TRUE);
		this->mainScope->registerVariable(KW_FALSE, Script::SCR_FALSE->getType(), Script::SCR_FALSE);

		this->mainScope->registerFunction(new ScriptFuncToString(*this->mainScope, "tostring"));

		ScriptClass *entrypoint = new ScriptClass(*this->mainScope, "entrypoint", nullptr);
		entrypoint->registerFunction(new ScriptMethod(
			*entrypoint,
			"run",
			ScriptType::create(VALUE_TYPE_T::VT_INT, *entrypoint),
			make_vector<FunctionParameter>() << ScriptType(VALUE_TYPE_T::VT_INT) << ScriptType::create(TypeDescriptor(VALUE_TYPE_T::VT_ARRAY, VALUE_TYPE_T::VT_STRING), *entrypoint),
			static_cast<BITFLAG_T>(MODIFIER_T::M_VIRTUAL) | static_cast<BITFLAG_T>(MODIFIER_T::M_ABSTRACT)
		));

		this->mainScope->registerClass(entrypoint);
	}

	int Script::run(int argc, char **argv)
	{
		DBG("Preparing to run the script");

		ScriptClass *ep = this->mainScope->findClassByInheritance("entrypoint");
		if (!ep)
			throw RuntimeException("Entrypoint class not found. A class extending \"entrypoint\" must be implemented");

		//ep->initialize();

		if (ep->isAbstract())
			throw CompileException(std::string("Cannot instatiate abstract class ") + ep->getName());

		ScriptMethod *em = ep->findMethod("run", make_vector<FunctionParameter>() << ScriptType::create(VALUE_TYPE_T::VT_INT, *ep) << ScriptType::create(TypeDescriptor(VALUE_TYPE_T::VT_ARRAY, VALUE_TYPE_T::VT_STRING), *ep));

		DBG("Running the script");

		ScriptClassInstance *inst = new ScriptClassInstance(*ep);

		ep->initialize(*inst);
		em->setClassInstance(inst);

		ScriptValue *returncode = em->run(make_vector<ScriptValue*>() << new ScriptIntValue(argc) << make_sarray(ScriptType::create(VALUE_TYPE_T::VT_STRING, *ep), argc, argv));

		delete inst;

		return static_cast<ScriptIntValue*>(returncode)->getValue();
	}

	void Script::import(const std::string &path)
	{
		Path p = Path(this->path);
		Script scr = Script::load(p.getDirectory().clone(path), this->mainScope);
		scr.isExtern = true;
		scr.compile();
	}

	const std::string &Script::getContent()
	{
		return this->content;
	}

	ScriptGlobalNamespace *Script::getMainScope()
	{
		return this->mainScope;
	}

	Script Script::load(const std::string &path, ScriptGlobalNamespace *parentMainScope)
	{
		std::ifstream str(path, std::ios_base::in | std::ios_base::ate);

		if (!str.is_open())
			throw CompileException("Source file \"" + path + "\" not found");

		std::streampos size = str.tellg();
		str.seekg(0, std::ios_base::beg);
		std::string scr(size, '\0');
		str.read(&scr[0], size);
		str.close();

		if (parentMainScope)
			return Script(path, scr, parentMainScope);

		return Script(path, scr);
	}
}