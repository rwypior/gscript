#include "common.h"
#include "gscript/runtime/while.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "RuntimeWhile")
{
	// Test variable
	auto& testVariable = globalNamespace.registerVariable("testVariable", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(0));

	// While condition
	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &testVariable);
	auto oplessthan = std::make_unique<gscript::ScriptOperatorLessThan>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH);
	auto literal10 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(10));
	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));
	stmtvec.push_back(std::move(oplessthan));
	stmtvec.push_back(std::move(literal10));
	auto cond = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvec));

	// For block
	auto varread2 = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &testVariable);
	auto incr = std::make_unique<gscript::ScriptOperatorIncrement>(globalNamespace, gscript::OPERATOR_LINK_T::OL_LEFT);
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varread2));
	stmtvecbody.push_back(std::move(incr));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	// While
	gscript::ScriptWhile f(globalNamespace, std::move(cond));
	f.merge(std::move(eb));

	REQUIRE(testVariable.getValue()->as<gscript::ScriptIntValue>().getValue() == 0);
	f.run();
	REQUIRE(testVariable.getValue()->as<gscript::ScriptIntValue>().getValue() == 10);
}