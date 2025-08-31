#include "common.h"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/funcCall.hpp"
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

TEST_CASE_METHOD(GscriptTest, "RuntimeStatement")
{
	// Variables
	auto& var1 = globalNamespace.registerVariable(
		std::make_unique<gscript::ScriptVariable>("var1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(1), 0));
	auto& var2 = globalNamespace.registerVariable(
		std::make_unique<gscript::ScriptVariable>("var2", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(42), 0));
	auto& var3 = globalNamespace.registerVariable(
		std::make_unique<gscript::ScriptVariable>("var3", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(1337), 0));

	// Statement body
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	// var1
	stmtvecbody.push_back(std::make_unique<gscript::ScriptVarRead>(globalNamespace, &var1));
	// add
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH));
	// var2
	stmtvecbody.push_back(std::make_unique<gscript::ScriptVarRead>(globalNamespace, &var2));
	// multiply
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorMultiply>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH));
	// 5
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(5)));
	// add
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH));
	// var2
	stmtvecbody.push_back(std::make_unique<gscript::ScriptVarRead>(globalNamespace, &var3));
	// multiply
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorMultiply>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH));
	// 10
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(10)));

	// Statement
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	auto result = stmt1->run();

	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 13581);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeStatementWithFuncCalls")
{
	// Function
	gscript::ScriptFunction myFunc(globalNamespace, "myfunc", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), {}, {});

	// Function block
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(myFunc, std::make_unique<gscript::ScriptIntValue>(42));
	auto funcstmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	funcstmtvecbody.push_back(std::move(literal42));
	auto funcstmt1 = std::make_unique<gscript::ScriptStatement>(myFunc, std::move(funcstmtvecbody));

	auto ret = std::make_unique<gscript::ScriptReturn>(myFunc, std::move(funcstmt1));

	auto stmtvecbodycallable = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbodycallable.push_back(std::move(ret));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbodycallable));

	myFunc.merge(std::move(eb));

	// Variables
	auto& var1 = globalNamespace.registerVariable(
		std::make_unique<gscript::ScriptVariable>("var2", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(42), 0));

	// Func call
	auto myfuncLink = gscript::DirectEntityLink<gscript::ScriptFunction*>(&myFunc);
	auto funccall = std::make_unique<gscript::ScriptFuncCall>(globalNamespace, &myfuncLink);

	// Statement body
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	// var1
	stmtvecbody.push_back(std::make_unique<gscript::ScriptVarRead>(globalNamespace, &var1));
	// add
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH));
	// func call
	stmtvecbody.push_back(std::move(funccall));
	// multiply
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorMultiply>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH));
	// 2
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(2)));

	// Statement
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	auto result = stmt1->run();

	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 126);
}