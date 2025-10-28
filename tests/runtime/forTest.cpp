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
	auto literal0 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(0));
	auto literal0vec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	literal0vec.push_back(std::move(literal0));
	auto stmtliteral = std::make_unique<gscript::ScriptStatement>(std::move(literal0vec));
	stmtliteral->setup(globalNamespace);
	auto& sv = globalNamespace.registerVariable("i", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(0));
	auto vardecl = std::make_unique<gscript::ScriptVarDeclaration>(globalNamespace, sv, std::move(stmtliteral));

	// For condition
	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &sv);
	auto oplessthan = std::make_unique<gscript::ScriptOperatorLessThan>(gscript::OperatorLinkage::Both);
	auto literal5 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(5));
	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));
	stmtvec.push_back(std::move(oplessthan));
	stmtvec.push_back(std::move(literal5));
	auto cond = std::make_unique<gscript::ScriptStatement>(std::move(stmtvec));
	cond->setup(globalNamespace);

	// For progress statement
	auto varread2 = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &sv);
	auto opincrement = std::make_unique<gscript::ScriptOperatorIncrement>(gscript::OperatorLinkage::Right);
	auto stmtvecprogress = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecprogress.push_back(std::move(opincrement));
	stmtvecprogress.push_back(std::move(varread2));
	auto progress = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecprogress));
	progress->setup(globalNamespace);

	// For block
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1));
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &myVariable1);
	auto mul = std::make_unique<gscript::ScriptOperatorAddTo>(gscript::OperatorLinkage::Both);
	auto varreadi = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &sv);
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(mul));
	stmtvecbody.push_back(std::move(varreadi));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
	stmt1->setup(globalNamespace);
	auto stmtvecbody1 = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	// The for itself
	gscript::ScriptFor f(std::move(vardecl), std::move(cond), std::move(progress));
	f.merge(std::move(eb));

	f.run(globalNamespace);

	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 5);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 11);
}