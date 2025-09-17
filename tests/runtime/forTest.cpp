#include "common.h"
#include "gscript/runtime/for.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "RuntimeFor")
{
	// For variable
	auto literal0 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(0));
	auto literal0vec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	literal0vec.push_back(std::move(literal0));
	auto stmtliteral = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(literal0vec));
	stmtliteral->setup();
	auto& sv = globalNamespace.registerVariable("i", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), std::make_unique<gscript::ScriptIntValue>(0));
	//gscript::ScriptVariable sv("i", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(0));
	auto vardecl = std::make_unique<gscript::ScriptVarDeclaration>(globalNamespace, sv, std::move(stmtliteral));

	// For condition
	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &sv);
	auto oplessthan = std::make_unique<gscript::ScriptOperatorLessThan>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH);
	auto literal5 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(5));
	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));
	stmtvec.push_back(std::move(oplessthan));
	stmtvec.push_back(std::move(literal5));
	auto cond = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvec));
	cond->setup();

	// For progress statement
	auto varread2 = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &sv);
	auto opincrement = std::make_unique<gscript::ScriptOperatorIncrement>(globalNamespace, gscript::OPERATOR_LINK_T::OL_RIGHT);
	auto stmtvecprogress = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecprogress.push_back(std::move(opincrement));
	stmtvecprogress.push_back(std::move(varread2));
	auto progress = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecprogress));
	progress->setup();

	// For block
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1));
	//gscript::ScriptVariable myVariable1("myVariable1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(1));
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &myVariable1);
	auto mul = std::make_unique<gscript::ScriptOperatorAddTo>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH);
	auto varreadi = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &sv);
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(mul));
	stmtvecbody.push_back(std::move(varreadi));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	stmt1->setup();
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	// The for itself
	gscript::ScriptFor f(globalNamespace, std::move(vardecl), std::move(cond), std::move(progress));
	f.merge(std::move(eb));

	f.run();

	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 5);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 11);
}