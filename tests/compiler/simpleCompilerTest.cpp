#include "common.h"
#include "gscript/compiler/compiler.hpp"
#include "gscript/parser/pFunction.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pNamespace.hpp"
#include "gscript/runtime/namespace.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/funcCall.hpp"
#include "gscript/runtime/class.hpp"
#include "gscript/runtime/classInstance.hpp"
#include "gscript/logger.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE_METHOD(GscriptTest, "CompilerFuncCall")
{
	std::string txt =
		"void myfunc() {\n"
		"	otherfunc();\n"
		"}\n"
		"void otherfunc() {\n"
		"}"
		;

	gscript::ParserNamespace mainNamespace(gscript::NamespaceType::Main);
	mainNamespace.parse(gscript::StringIteratorRange(txt.begin(), txt.end(), "", 0));

	gscript::Compiler compiler;

	for (auto& fnc : mainNamespace.functions)
	{
		globalNamespace.registerFunction(compiler.compileFunction(&globalNamespace, fnc));
	}

	auto myfunc = globalNamespace.getFunction("myfunc", {});
	auto otherfunc = globalNamespace.getFunction("otherfunc", {});

	REQUIRE(myfunc);
	REQUIRE(otherfunc);

	REQUIRE(myfunc->getStatements().size() == 1);
	REQUIRE(otherfunc->getStatements().size() == 0);

	auto* stmt0_prototype = static_cast<gscript::ScriptStatement*>(myfunc->getStatements().at(0).get());
	auto* stmt0_call0_prototype = dynamic_cast<gscript::ScriptFuncCallPrototype*>(stmt0_prototype->callable.get());

	REQUIRE(stmt0_call0_prototype);
	REQUIRE(stmt0_call0_prototype->getName() == "otherfunc");

	compiler.finalize(globalNamespace);

	auto* stmt0 = static_cast<gscript::ScriptStatement*>(myfunc->getStatements().at(0).get());
	auto* stmt0_call0 = static_cast<gscript::ScriptFuncCall*>(stmt0->callable.get());

	REQUIRE(stmt0_call0);
	REQUIRE(stmt0_call0->getFunc().get()->getName() == "otherfunc");
}


TEST_CASE_METHOD(GscriptTest, "CompilerEmptyFunctions")
{
	std::string txt =
		"void myfunc() {\n"
		"}\n"
		"void otherfunc() {\n"
		"}"
		;

	gscript::ParserNamespace mainNamespace(gscript::NamespaceType::Main);
	mainNamespace.parse(gscript::StringIteratorRange(txt.begin(), txt.end(), "", 0));

	gscript::Compiler compiler;

	for (auto& fnc : mainNamespace.functions)
	{
		globalNamespace.registerFunction(compiler.compileFunction(&globalNamespace, fnc));
	}

	auto myfunc = globalNamespace.getFunction("myfunc", {});
	auto otherfunc = globalNamespace.getFunction("otherfunc", {});

	REQUIRE(myfunc);
	REQUIRE(otherfunc);
}

TEST_CASE_METHOD(GscriptTest, "CompilerClassVarRead")
{
	std::string txt =
		"class myClass {\n"
		"	int fnc() {\n"
		"		return myvar;\n"
		"	}\n"
		"	int myvar = 42;\n"
		"}"
		;

	gscript::ParserNamespace mainNamespace(gscript::NamespaceType::Main);
	mainNamespace.parse(gscript::StringIteratorRange(txt.begin(), txt.end(), "", 0));

	gscript::Compiler compiler;

	for (auto& cls : mainNamespace.classes)
	{
		globalNamespace.registerClass(compiler.compileClass(&globalNamespace, cls));
	}

	auto myClass = globalNamespace.findClass("myClass");
	auto myClassObject = myClass->instantiate();

	REQUIRE(myClassObject);

	auto myvar = myClassObject->getVariable("myvar");

	REQUIRE(myvar);
	REQUIRE(myvar->getName() == "myvar");
	REQUIRE(myvar->getValue()->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "CompilerClassInheritance")
{
	std::string txt =
		"class base {\n"
		"	int fnc() {\n"
		"		return 42;\n"
		"	}\n"
		"}\n"
		"class myClass : base {\n"
		"}"
		;

	gscript::ParserNamespace mainNamespace(gscript::NamespaceType::Main);
	mainNamespace.parse(gscript::StringIteratorRange(txt.begin(), txt.end(), "", 0));

	gscript::Compiler compiler;

	for (auto& cls : mainNamespace.classes)
	{
		globalNamespace.registerClass(compiler.compileClass(&globalNamespace, cls));
	}

	auto myClass = globalNamespace.findClass("myClass");
	REQUIRE(myClass->getBase()->getName() == "base");
	
	auto myClassObject = myClass->instantiate();

	REQUIRE(myClassObject);

	auto fnc = myClassObject->findFunction("fnc", {});

	REQUIRE(fnc);
	REQUIRE(fnc->getName() == "fnc");
}

TEST_CASE_METHOD(GscriptTest, "CompilerObjectCall")
{
	std::string txt =
		"class myClass {\n"
		"	int fnc() {\n"
		"		return 42;\n"
		"	}\n"
		"}\n"
		"int test() {\n"
		"	myClass x = new myClass();\n"
		"	return x.fnc();\n"
		"}"
		;

	gscript::ParserNamespace mainNamespace(gscript::NamespaceType::Main);
	mainNamespace.parse(gscript::StringIteratorRange(txt.begin(), txt.end(), "", 0));

	gscript::Compiler compiler;

	for (auto& cls : mainNamespace.classes)
	{
		globalNamespace.registerClass(compiler.compileClass(&globalNamespace, cls));
	}

	for (auto& fnc : mainNamespace.functions)
	{
		globalNamespace.registerFunction(compiler.compileFunction(&globalNamespace, fnc));
	}

	gs_debug_log();

	compiler.finalize(globalNamespace);

	auto testFnc = globalNamespace.findFunction("test", {});
	auto res = testFnc->run(*testFnc);

	REQUIRE(res->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "CompilerVirtualCall")
{
	std::string txt =
		"class base {\n"
		"	virtual int fnc() {\n"
		"		return 42;\n"
		"	}\n"
		"}\n"
		"class myClass : base {\n"
		"	int fnc() {\n"
		"		return 1337;\n"
		"	}\n"
		"}\n"
		"int test() {\n"
		"	base x = new myClass();\n"
		"	return x.fnc();\n"
		"}"
		;

	gscript::ParserNamespace mainNamespace(gscript::NamespaceType::Main);
	mainNamespace.parse(gscript::StringIteratorRange(txt.begin(), txt.end(), "", 0));

	gscript::Compiler compiler;

	for (auto& cls : mainNamespace.classes)
	{
		globalNamespace.registerClass(compiler.compileClass(&globalNamespace, cls));
	}

	for (auto& fnc : mainNamespace.functions)
	{
		globalNamespace.registerFunction(compiler.compileFunction(&globalNamespace, fnc));
	}

	compiler.finalize(globalNamespace);

	auto testFnc = globalNamespace.findFunction("test", {});
	auto res = testFnc->run(*testFnc);

	REQUIRE(res->as<gscript::ScriptIntValue>().getValue() == 1337);
}