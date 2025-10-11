#include "script.hpp"

#include "parser/pClass.hpp"
#include "parser/pNamespace.hpp"
#include "parser/pStatement.hpp"
#include "parser/pVarDeclaration.hpp"

#include "runtime/scriptValue.hpp"
#include "runtime/classInstance.hpp"

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
#include <sstream>
#include <vector>
#include <memory>

namespace gscript
{
	Script::Script()
		: mainScope(std::make_shared<ScriptNamespace>())
	{
	}

	Script::Script(const std::string &path)
		: path(path)
		, mainScope(std::make_shared<ScriptNamespace>())
	{
	}

	Script::Script(const std::string &path, std::shared_ptr<ScriptNamespace> mainScope)
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

		ParserNamespace mainNamespace(NamespaceType::Main);
		mainNamespace.parse(StringIteratorRange(source.begin(), source.end(), this->path, 0));

		for (auto& ex : mainNamespace.extensions)
		{
			if (ScriptExtension *ext = this->findExtension(ex))
				ext->load(*this);
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

		for (auto& fnc : mainNamespace.functions)
		{
			this->mainScope->registerFunction(compiler.compileFunction(this->mainScope.get(), fnc));
			//this->mainScope->registerFunctionPrototype(*it);
		}

		this->mainScope->resolveFunctions();
		this->mainScope->resolveClassMembers();

		compiler.finalize(*this->mainScope);

		return true;
	}

	void Script::init()
	{
		this->mainScope->registerVariable(keywordNull, ScriptType::nulltype(), ScriptType::null());
		this->mainScope->registerVariable(keywordTrue, ScriptType::booltype(), ScriptType::btrue());
		this->mainScope->registerVariable(keywordFalse, ScriptType::booltype(), ScriptType::bfalse());

		this->mainScope->registerFunction(std::make_unique<ScriptFuncToString>(*this->mainScope, "tostring"));

		auto entrypoint = std::make_unique<ScriptClass>(*this->mainScope, entryPointClass, nullptr);
		entrypoint->registerFunction(std::make_unique<ScriptMethod>(
			*entrypoint,
			"run",
			ScriptType::create(ValueType::Int, *entrypoint),
			std::vector<FunctionParameter> {
				std::make_shared<ScriptType>(ValueType::Int),
				std::shared_ptr<ScriptType>(ScriptType::create(TypeDescriptor(ValueType::Array, ValueType::String), *entrypoint))
			},
			Modifier::Virtual | Modifier::Abstract
		));

		this->mainScope->registerClass(std::move(entrypoint));
	}

	int Script::run(int argc, char **argv)
	{
		DBG("Preparing to run the script");

		ScriptClass *ep = this->mainScope->findClassByInheritance(entryPointClass);
		if (!ep)
			throw RuntimeException("Entrypoint class not found. A class extending \"entrypoint\" must be implemented");

		if (ep->isAbstract())
			throw CompileException(std::string("Cannot instatiate abstract class ") + ep->getName());

		ScriptMethod *em = ep->findMethod(
			"run", 
			std::vector<FunctionParameter> {
				std::shared_ptr<ScriptType>(ScriptType::create(ValueType::Int, *ep)),
				std::shared_ptr<ScriptType>(ScriptType::create(TypeDescriptor(ValueType::Array, ValueType::String), *ep))
			},
			false, false
		);

		if (!em)
			throw RuntimeException((std::stringstream() << "Class \"" << ep->getName() << "\" is required to provide \"run(int, string[])\" method").str());

		DBG("Running the script");

		auto inst = std::make_unique<ScriptClassValue>(ep->instantiate());

		std::vector<std::unique_ptr<ScriptValue>> params;

		for (size_t i = 0; i < argc; i++)
		{
			params.push_back(std::make_unique<ScriptStringValue>(argv[i]));
		}

		std::vector<std::unique_ptr<ScriptValue>> paramvalues;
		paramvalues.push_back(std::make_unique<ScriptIntValue>(argc));
		paramvalues.push_back(std::make_unique<ScriptArrayValue>(std::move(params)));

		auto returncode = em->instrun(std::make_unique<gscript::ScriptReferenceValue>(inst.get()), std::move(paramvalues));

		return static_cast<ScriptIntValue*>(returncode.get())->getValue();
	}

	void Script::import(const std::string &path)
	{
		Path p = Path(this->path);
		Path importPath = p.getDirectory().clone(path);
		Script scr(importPath, this->mainScope);
		scr.compile();
	}

	ScriptNamespace *Script::getMainScope()
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