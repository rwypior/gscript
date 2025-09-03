#include "common.h"
#include "gscript/compiler/compiler.hpp"
#include "gscript/parser/pFunction.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pNamespace.hpp"
#include "gscript/runtime/namespace.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/funcCall.hpp"

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

	gscript::ParserNamespace mainNamespace(gscript::NAMESPACE_TYPE_T::NT_MAIN);
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

	auto stmt0 = std::static_pointer_cast<gscript::ScriptStatement>(myfunc->getStatements().at(0));
	auto stmt0_call0 = std::static_pointer_cast<gscript::ScriptFuncCall>(stmt0->callable);
	//REQUIRE(stmt0_call0->getFunc()->get()->getName() == "otherfunc");
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

	gscript::ParserNamespace mainNamespace(gscript::NAMESPACE_TYPE_T::NT_MAIN);
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