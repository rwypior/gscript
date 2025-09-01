#include "common.h"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/return.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "RuntimeFunctionSimple")
{
	// Function
	gscript::ScriptFunction myFunc(globalNamespace, "myfunc", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), {}, {});

	// Function block
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(1));

	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(myFunc, &myVariable1);
	auto mul = std::make_unique<gscript::ScriptOperatorMultiplyBy>(myFunc, gscript::OPERATOR_LINK_T::OL_BOTH);
	auto literal5 = std::make_unique<gscript::ScriptLiteral>(myFunc, std::make_unique<gscript::ScriptIntValue>(5));
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(mul));
	stmtvecbody.push_back(std::move(literal5));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(stmtvecbody));
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	myFunc.merge(std::move(eb));

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 1);

	myFunc.run();
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 5);

	myFunc.run();
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 25);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeFunctionReturn")
{
	// Function
	gscript::ScriptFunction myFunc(globalNamespace, "myfunc", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), {}, {});

	// Function block
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(myFunc, std::make_unique<gscript::ScriptIntValue>(42));
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(literal42));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(stmtvecbody));

	auto ret = std::make_unique<gscript::ScriptReturn>(myFunc, std::move(stmt1));

	auto stmtvecbodycallable = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbodycallable.push_back(std::move(ret));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbodycallable));

	myFunc.merge(std::move(eb));

	auto result = myFunc.run();
	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 42);

	result = myFunc.run();
	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 42);

	result = myFunc.run();
	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 42);
}