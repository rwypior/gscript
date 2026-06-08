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
#include "gscript/runtime/varRead.hpp"
#include "gscript/logger.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE_METHOD(GscriptTest, "Compiler::Prototype::ResolvingNestedFuncCall")
{
	std::string txt =
		"int getint() {\n"
		"	return 42;\n"
		"}\n"
		"string getstr(int i) {\n"
		"	return \"test\";\n"
		"}\n"
		"void myfunc() {\n"
		"	otherfunc(getstr(getint()));\n"
		"}\n"
		"void otherfunc(string str) {\n"
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
	auto otherfunc = globalNamespace.getFunction("otherfunc", { std::make_shared<gscript::ScriptType>(gscript::ValueType::String) } );

	REQUIRE(myfunc);
	REQUIRE(otherfunc);

	REQUIRE(myfunc->getStatements().size() == 1);
	REQUIRE(otherfunc->getStatements().size() == 0);

	auto* stmt0_prototype = static_cast<gscript::ScriptStatement*>(myfunc->getStatements().at(0).get());
	auto* stmt0_call0_prototype = dynamic_cast<gscript::ScriptFuncCallPrototype*>(stmt0_prototype->callable.get());

	REQUIRE(stmt0_call0_prototype);
	REQUIRE(stmt0_call0_prototype->getName() == "otherfunc");
	REQUIRE(dynamic_cast<gscript::ScriptFuncCallPrototype*>(stmt0_call0_prototype->getParams().at(0)->callable.get()));

	compiler.finalize(globalNamespace);

	auto* stmt0 = static_cast<gscript::ScriptStatement*>(myfunc->getStatements().at(0).get());
	auto* stmt0_call0 = static_cast<gscript::ScriptFuncCall*>(stmt0->callable.get());

	REQUIRE(stmt0_call0);
	REQUIRE(stmt0_call0->getFunc().get()->getName() == "otherfunc");
}

TEST_CASE_METHOD(GscriptTest, "Compiler::Prototype::ResolvingVariablesReads")
{
	std::string txt =
		"void somefunc() {\n"
		"	int a = 5;\n"
		"	int b = a;\n"
		"}"
		;

	gscript::ParserNamespace mainNamespace(gscript::NamespaceType::Main);
	mainNamespace.parse(gscript::StringIteratorRange(txt.begin(), txt.end(), "", 0));

	gscript::Compiler compiler;

	for (auto& fnc : mainNamespace.functions)
	{
		globalNamespace.registerFunction(compiler.compileFunction(&globalNamespace, fnc));
	}

	auto myfunc = globalNamespace.getFunction("somefunc", {});

	REQUIRE(myfunc);

	REQUIRE(myfunc->getStatements().size() == 2);

	auto stmt1_decl0 = static_cast<gscript::ScriptVarDeclaration*>(myfunc->getStatements().at(1).get());
	auto stmt1_decl0_stmt0 = static_cast<gscript::ScriptStatement*>(stmt1_decl0->getStatement().get());
	auto stmt1_decl0_stmt0_varreadprototype = dynamic_cast<gscript::ScriptVarReadPrototype*>(stmt1_decl0_stmt0->callable.get());

	REQUIRE(stmt1_decl0_stmt0_varreadprototype); // Before finalization var read is still a prototype
	REQUIRE(stmt1_decl0_stmt0_varreadprototype->getName() == "a");

	compiler.finalize(globalNamespace);

	auto after_stmt1_decl0 = static_cast<gscript::ScriptVarDeclaration*>(myfunc->getStatements().at(1).get());
	auto after_stmt1_decl0_stmt0 = static_cast<gscript::ScriptStatement*>(stmt1_decl0->getStatement().get());
	auto after_stmt1_decl0_stmt0_varreadprototype = dynamic_cast<gscript::ScriptVarReadPrototype*>(stmt1_decl0_stmt0->callable.get());

	REQUIRE(after_stmt1_decl0_stmt0_varreadprototype == nullptr); // Var read is no longer a prototype
}