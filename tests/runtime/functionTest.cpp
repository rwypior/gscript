#include "common.h"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/return.hpp"
#include "gscript/runtime/if.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "RuntimeFunctionSimple")
{
	// Function
	gscript::ScriptFunction myFunc(globalNamespace, "myfunc", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), {}, {});

	// Function block
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1));

	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(myFunc, &myVariable1);
	auto mul = std::make_unique<gscript::ScriptOperatorMultiplyBy>(myFunc, gscript::OperatorLinkage::Both);
	auto literal5 = std::make_unique<gscript::ScriptLiteral>(myFunc, std::make_unique<gscript::ScriptIntValue>(5));
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(mul));
	stmtvecbody.push_back(std::move(literal5));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(stmtvecbody));
	stmt1->setup();
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
	gscript::ScriptFunction myFunc(globalNamespace, "myfunc", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), {}, {});

	// Function block
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(myFunc, std::make_unique<gscript::ScriptIntValue>(42));
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(literal42));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(stmtvecbody));
	stmt1->setup();

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

TEST_CASE_METHOD(GscriptTest, "RuntimeFunctionWithParams")
{
	// Function
	gscript::ScriptFunction myFunc(
		globalNamespace, 
		"myfunc", 
		gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace),
		{ { std::make_shared<gscript::ScriptType>(gscript::ValueType::Int), "arg1" }},
		{}
	);

	// Function block
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1));

	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(myFunc, &myVariable1);
	auto assign = std::make_unique<gscript::ScriptOperatorAssign>(myFunc, gscript::OperatorLinkage::Both);
	auto varreadParam1 = std::make_unique<gscript::ScriptVarReadPrototype>(myFunc, "arg1");
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(assign));
	stmtvecbody.push_back(std::move(varreadParam1));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(stmtvecbody));
	stmt1->setup();
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	myFunc.merge(std::move(eb));
	myFunc.setup();

	// Test

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 1);

	std::vector<std::unique_ptr<gscript::ScriptValue>> params1;
	params1.push_back(std::make_unique<gscript::ScriptIntValue>(42));
	myFunc.run(std::move(params1));
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 42);

	std::vector<std::unique_ptr<gscript::ScriptValue>> params2;
	params2.push_back(std::make_unique<gscript::ScriptIntValue>(1337));
	myFunc.run(std::move(params2));
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 1337);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeFunctionRecursive")
{
	// Function
	gscript::ScriptFunction myFunc(
		globalNamespace,
		"myfunc",
		gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace),
		{ { std::make_shared<gscript::ScriptType>(gscript::ValueType::Int), "arg1" } },
		{}
	);

	// If

	auto ifvarread = std::make_unique<gscript::ScriptVarReadPrototype>(myFunc, "arg1");
	auto ifgreater = std::make_unique<gscript::ScriptOperatorGreaterThan>(myFunc, gscript::OperatorLinkage::Both);
	auto if10 = std::make_unique<gscript::ScriptLiteral>(myFunc, std::make_unique<gscript::ScriptIntValue>(10));
	std::vector<std::unique_ptr<gscript::ScriptCallable>> ifcallables;
	ifcallables.push_back(std::move(ifvarread));
	ifcallables.push_back(std::move(ifgreater));
	ifcallables.push_back(std::move(if10));
	auto ifstmt = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(ifcallables));
	ifstmt->setup();

	// If block

	auto ifblockvarread = std::make_unique<gscript::ScriptVarReadPrototype>(myFunc, "arg1");
	std::vector<std::unique_ptr<gscript::ScriptCallable>> ifblockcallables;
	ifblockcallables.push_back(std::move(ifblockvarread));
	auto ifblockstmt = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(ifblockcallables));
	auto ifblockreturn = std::make_unique<gscript::ScriptReturn>(myFunc, std::move(ifblockstmt));
	std::vector<std::shared_ptr<gscript::ScriptCallable>> ifblockstatements;
	ifblockstatements.push_back(std::move(ifblockreturn));

	auto sif = std::make_unique<gscript::ScriptIf>(myFunc, std::move(ifstmt), nullptr, std::move(ifblockstatements));
	sif->setup();

	// Return

	auto returnvarread = std::make_unique<gscript::ScriptVarReadPrototype>(myFunc, "arg1");
	auto returnvarread2 = std::make_unique<gscript::ScriptVarReadPrototype>(myFunc, "arg1");
	auto returnmul = std::make_unique<gscript::ScriptOperatorMultiply>(myFunc, gscript::OperatorLinkage::Both);
	std::vector<std::unique_ptr<gscript::ScriptCallable>> returncallables;
	returncallables.push_back(std::move(returnvarread));
	returncallables.push_back(std::move(returnmul));
	returncallables.push_back(std::move(returnvarread2));
	auto returnstmt = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(returncallables));
	returnstmt->setup();

	auto elsereturn = std::make_unique<gscript::ScriptReturn>(myFunc, std::move(returnstmt));
	elsereturn->setup();

	// Function statements

	auto stmtvecbody = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(sif));
	stmtvecbody.push_back(std::move(elsereturn));

	// TODO - if this code was in place - it would result in weird, hard-to-debug error,
	// add some asserts to prevent it
	// 
	//auto stmt1 = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(stmtvecbody));
	//stmt1->setup();
	//auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	//stmtvecbody1.push_back(std::move(stmt1));
	//
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody));

	myFunc.merge(std::move(eb));
	myFunc.setup();

	// Test

	std::vector<std::unique_ptr<gscript::ScriptValue>> params;
	params.push_back(std::make_unique<gscript::ScriptIntValue>(3));
	auto res = myFunc.run(std::move(params));

	REQUIRE(res->as<gscript::ScriptIntValue>().getValue() == 81);
}