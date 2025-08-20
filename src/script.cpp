#include "script.hpp"

#include "parser/pClass.hpp"
#include "parser/pNamespace.hpp"
#include "parser/pStatement.hpp"
#include "parser/pVarDeclaration.hpp"

#include "runtime/scriptValue.hpp"
#include "runtime/classInstance.hpp"
#include "runtime/globalNamespace.hpp"

#include "framework/sfPrint.hpp"
#include "framework/sfToString.hpp"

#include "extension/scriptExtension.hpp"
#include "extension/ConsoleExtension.hpp"
#include "extension/IOExtension.hpp"

#include "compiler/compiler.hpp"

#include "IteratorRange.hpp"
#include "defs.hpp"
#include "type.hpp"
#include "path.hpp"
#include "util.hpp"
#include "compileException.hpp"
#include "runtimeException.hpp"
#include "debug.hpp"

#include <iostream>
#include <fstream>
#include <string>

namespace gscript
{
	Script::Script()
		: mainScope(std::make_unique<ScriptGlobalNamespace>(*this))
	{
	}

	Script::Script(const std::string &path)
		: path(path)
		, mainScope(std::make_unique<ScriptGlobalNamespace>(*this))
	{
	}

	Script::Script(const std::string &path, std::shared_ptr<ScriptGlobalNamespace> mainScope)
		: path(path)
		, mainScope(mainScope)
		, isExtern(true)
	{
	}

	Script::Script(const std::string &path, Script &script)
		: path(path)
		, mainScope(script.mainScope)
		, extensions(script.extensions)
		, isExtern(true)
	{
	}

	void Script::extend(ScriptExtension *extension, const std::string &name)
	{
		this->extensions.emplace(name.empty() ? extension->getName() : name, extension);
	}

	void Script::loadDefaultExtensions()
	{
		this->extend(new ConsoleExtension());
		this->extend(new IOExtension());
	}

	ScriptExtension *Script::findExtension(const std::string &name)
	{
		auto it = this->extensions.find(name);
		if (it == this->extensions.end())
			return nullptr;

		return it->second.get();
	}

	bool Script::compile()
	{
		std::string source = loadSource(this->path);

		Compiler compiler;

		ParserNamespace mainNamespace(NAMESPACE_TYPE_T::NT_MAIN);
		mainNamespace.parse(StringIteratorRange(source.begin(), source.end()));

		for (auto& ex : mainNamespace.extensions)
		{
			if (ScriptExtension *ext = this->mainScope->getScript().findExtension(ex))
				ext->load(this->mainScope->getScript());
			else
				throw CompileException(std::string("Extension \"" + ex + "\" could not be found"));
		}

		if (!this->isExtern)
			this->init();
		
		for (auto& ns : mainNamespace.namespaces)
		{
			this->mainScope->registerNamespace(compiler.compileNamespace(this->mainScope.get(), ns));
			//this->mainScope->registerNamespace(*it);
		}

		for (auto& imp : mainNamespace.imports)
		{
			this->import(imp);
		}

		for (auto& cls : mainNamespace.classes)
		{
			//this->mainScope->registerClassPrototype(*it);
			this->mainScope->registerClass(compiler.compileClass(this->mainScope.get(), cls));
		}

		// this->mainScope->resolveClasses(); // TODO

		for (ParserNamespace::FUNCTION_CONTAINER_T::iterator it = mainNamespace.functions.begin(); it != mainNamespace.functions.end(); ++it)
		for (auto& fnc : mainNamespace.functions)
		{
			this->mainScope->registerFunction(compiler.compileFunction(this->mainScope.get(), fnc));
			//this->mainScope->registerFunctionPrototype(*it);
		}

		this->mainScope->resolveFunctions();
		this->mainScope->resolveClassMembers();

		return true;
	}

	void Script::init()
	{
		this->mainScope->registerVariable(KW_NULL, SCR_NULL->getType(), SCR_NULL);
		this->mainScope->registerVariable(KW_TRUE, SCR_TRUE->getType(), SCR_TRUE);
		this->mainScope->registerVariable(KW_FALSE, SCR_FALSE->getType(), SCR_FALSE);

		this->mainScope->registerFunction(std::make_unique<ScriptFuncToString>(*this->mainScope, "tostring"));

		auto entrypoint = std::make_unique<ScriptClass>(*this->mainScope, "entrypoint", nullptr);
		entrypoint->registerFunction(std::make_unique<ScriptMethod>(
			*entrypoint,
			"run",
			ScriptType::create(VALUE_TYPE_T::VT_INT, *entrypoint),
			make_vector<FunctionParameter>() << ScriptType(VALUE_TYPE_T::VT_INT) << ScriptType::create(TypeDescriptor(VALUE_TYPE_T::VT_ARRAY, VALUE_TYPE_T::VT_STRING), *entrypoint),
			static_cast<BITFLAG_T>(MODIFIER_T::M_VIRTUAL) | static_cast<BITFLAG_T>(MODIFIER_T::M_ABSTRACT)
		));

		this->mainScope->registerClass(std::move(entrypoint));
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
		Path importPath = p.getDirectory().clone(path);
		Script scr(importPath, this->mainScope);
		scr.compile();
	}

	ScriptGlobalNamespace *Script::getMainScope()
	{
		return this->mainScope.get();
	}

	std::string Script::loadSource(const std::string &path)
	{
		std::ifstream str(path, std::ios_base::in | std::ios_base::ate);

		if (!str.is_open())
			throw CompileException("Source file \"" + path + "\" not found");

		std::streampos size = str.tellg();
		str.seekg(0, std::ios_base::beg);
		std::string scr(size, '\0');
		str.read(&scr[0], size);
		str.close();

		return scr;
	}
}