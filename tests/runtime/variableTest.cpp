#include "common.h"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "Variable declaration")
{
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(42));
	auto literal42vec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	literal42vec.push_back(std::move(literal42));

	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(literal42vec));

	gscript::ScriptVariable sv("myVariable1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(0), 0);
	gscript::ScriptVarDeclaration vd(globalNamespace, sv, std::move(stmt1));

	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 0);

	vd.run();

	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "Registering variables")
{
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(42));
	auto literal42vec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	literal42vec.push_back(std::move(literal42));

	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(literal42vec));

	auto sv = std::make_unique<gscript::ScriptVariable>("myVariable1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(0), 0);

	auto myVariable1 = globalNamespace.findVariable("myVariable1");
	REQUIRE(myVariable1 == nullptr);
	REQUIRE(sv);

	globalNamespace.registerVariable(std::move(sv));

	myVariable1 = globalNamespace.findVariable("myVariable1");
	REQUIRE(myVariable1 != nullptr);
	REQUIRE(!sv);

	REQUIRE(myVariable1->getValue()->as<gscript::ScriptIntValue>().getValue() == 0);

	gscript::ScriptVarDeclaration vd(globalNamespace, *myVariable1, std::move(stmt1));
	vd.run();

	REQUIRE(myVariable1->getValue()->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "Modifying variables")
{
	gscript::ScriptVariable sv("myVariable1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(0), 0);

	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &sv);
	auto assign = std::make_unique<gscript::ScriptOperatorAssign>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH);
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(42));

	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));
	stmtvec.push_back(std::move(assign));
	stmtvec.push_back(std::move(literal42));

	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvec));
	
	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 0);

	stmt1->run();

	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 42);
}