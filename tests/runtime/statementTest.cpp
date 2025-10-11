#include "common.h"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/funcCall.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/return.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "RuntimeStatement")
{
	// Variables
	auto& var1 = globalNamespace.registerVariable(
		std::make_unique<gscript::ScriptVariable>("var1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1)));
	auto& var2 = globalNamespace.registerVariable(
		std::make_unique<gscript::ScriptVariable>("var2", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42)));
	auto& var3 = globalNamespace.registerVariable(
		std::make_unique<gscript::ScriptVariable>("var3", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1337)));

	// Statement body
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	// var1
	stmtvecbody.push_back(std::make_unique<gscript::ScriptVarRead>(globalNamespace, &var1));
	// add
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(globalNamespace, gscript::OperatorLinkage::Both));
	// var2
	stmtvecbody.push_back(std::make_unique<gscript::ScriptVarRead>(globalNamespace, &var2));
	// multiply
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorMultiply>(globalNamespace, gscript::OperatorLinkage::Both));
	// 5
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(5)));
	// add
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(globalNamespace, gscript::OperatorLinkage::Both));
	// var2
	stmtvecbody.push_back(std::make_unique<gscript::ScriptVarRead>(globalNamespace, &var3));
	// multiply
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorMultiply>(globalNamespace, gscript::OperatorLinkage::Both));
	// 10
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(10)));

	// Statement
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	stmt1->setup(globalNamespace);
	auto result = stmt1->run(globalNamespace);

	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 13581);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeStatementWithFuncCalls")
{
	// Function
	auto& myFunc = globalNamespace.registerFunction("myfunc", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), {}, {});

	// Function block
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(myFunc, std::make_unique<gscript::ScriptIntValue>(42));
	auto funcstmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	funcstmtvecbody.push_back(std::move(literal42));
	auto funcstmt1 = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(funcstmtvecbody));
	funcstmt1->setup(globalNamespace);

	auto ret = std::make_unique<gscript::ScriptReturn>(myFunc, std::move(funcstmt1));

	auto stmtvecbodycallable = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbodycallable.push_back(std::move(ret));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbodycallable));

	myFunc.merge(std::move(eb));

	// Variables
	auto& var1 = globalNamespace.registerVariable(
		std::make_unique<gscript::ScriptVariable>("var2", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42)));

	// Func call
	auto funccall = std::make_unique<gscript::ScriptFuncCall>(globalNamespace, &myFunc);

	// Statement body
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	// var1
	stmtvecbody.push_back(std::make_unique<gscript::ScriptVarRead>(globalNamespace, &var1));
	// add
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(globalNamespace, gscript::OperatorLinkage::Both));
	// func call
	stmtvecbody.push_back(std::move(funccall));
	// multiply
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorMultiply>(globalNamespace, gscript::OperatorLinkage::Both));
	// 2
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(2)));

	// Statement
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	stmt1->setup(globalNamespace);
	auto result = stmt1->run(globalNamespace);

	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 126);
}