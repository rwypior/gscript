#include "common.h"
#include "gscript/runtime/if.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "RuntimeIfVariable")
{
	// Test variable
	auto& testVariable = globalNamespace.registerVariable("testVariable", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42));

	// Variable
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42));

	// If condition
	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &testVariable);
	auto opeq = std::make_unique<gscript::ScriptOperatorEquals>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(42));
	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));
	stmtvec.push_back(std::move(opeq));
	stmtvec.push_back(std::move(literal42));
	auto cond = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvec));
	cond->setup(globalNamespace);

	// If block
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &myVariable1);
	auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal10 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(10));

	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(opadd));
	stmtvecbody.push_back(std::move(literal10));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	stmt1->setup(globalNamespace);
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	// If
	gscript::ScriptIf f(globalNamespace, std::move(cond));
	f.merge(std::move(eb));

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 42);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 52);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 62);

	testVariable.setValue(std::make_unique<gscript::ScriptIntValue>(1337));

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 62);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 62);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeIfElseVariable")
{
	// Test variable
	auto& testVariable = globalNamespace.registerVariable("testVariable", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42));

	// Variable
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42));

	// If condition
	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &testVariable);
	auto opeq = std::make_unique<gscript::ScriptOperatorEquals>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(42));
	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));
	stmtvec.push_back(std::move(opeq));
	stmtvec.push_back(std::move(literal42));
	auto cond = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvec));
	cond->setup(globalNamespace);

	// If block
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &myVariable1);
	auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal10 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(10));

	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(opadd));
	stmtvecbody.push_back(std::move(literal10));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	stmt1->setup(globalNamespace);
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	// Else block
	auto varreadMyVariable1else = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &myVariable1);
	auto opmulelse = std::make_unique<gscript::ScriptOperatorMultiplyBy>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal10else = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(10));

	auto stmtvecbodyelse = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbodyelse.push_back(std::move(varreadMyVariable1else));
	stmtvecbodyelse.push_back(std::move(opmulelse));
	stmtvecbodyelse.push_back(std::move(literal10else));
	auto stmt1else = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbodyelse));
	stmt1else->setup(globalNamespace);
	auto stmtvecbody1else = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1else.push_back(std::move(stmt1else));
	auto ebelse = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1else));

	// Else
	auto selse = std::make_unique<gscript::ScriptIf>(globalNamespace, nullptr, nullptr);
	selse->merge(std::move(ebelse));

	// If
	gscript::ScriptIf f(globalNamespace, std::move(cond), std::move(selse));
	f.merge(std::move(eb));

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 42);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 52);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 62);

	testVariable.setValue(std::make_unique<gscript::ScriptIntValue>(1337));

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 620);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6200);

	testVariable.setValue(std::make_unique<gscript::ScriptIntValue>(42));

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6210);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6220);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeIfElseIfElseVariable")
{
	// Test variable
	auto& testVariable = globalNamespace.registerVariable("testVariable", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42));

	// Variable
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42));

	// If condition
	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &testVariable);
	auto opeq = std::make_unique<gscript::ScriptOperatorEquals>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(42));
	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));
	stmtvec.push_back(std::move(opeq));
	stmtvec.push_back(std::move(literal42));
	auto cond = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvec));
	cond->setup(globalNamespace);

	// If block
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &myVariable1);
	auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal10 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(10));

	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(opadd));
	stmtvecbody.push_back(std::move(literal10));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	stmt1->setup(globalNamespace);
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	// Else block
	auto varreadMyVariable1else = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &myVariable1);
	auto opmulelse = std::make_unique<gscript::ScriptOperatorMultiplyBy>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal10else = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(10));

	auto stmtvecbodyelse = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbodyelse.push_back(std::move(varreadMyVariable1else));
	stmtvecbodyelse.push_back(std::move(opmulelse));
	stmtvecbodyelse.push_back(std::move(literal10else));
	auto stmt1else = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbodyelse));
	stmt1else ->setup(globalNamespace);
	auto stmtvecbody1else = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1else.push_back(std::move(stmt1else));
	auto ebelse = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1else));

	// Else
	auto selse = std::make_unique<gscript::ScriptIf>(globalNamespace, nullptr, nullptr);
	selse->merge(std::move(ebelse));

	// Else if condition
	auto varreadelseif = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &testVariable);
	auto opeqelseif = std::make_unique<gscript::ScriptOperatorEquals>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal1337elseif = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(1337));
	auto stmtvecelseif = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecelseif.push_back(std::move(varreadelseif));
	stmtvecelseif.push_back(std::move(opeqelseif));
	stmtvecelseif.push_back(std::move(literal1337elseif));
	auto condelseif = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecelseif));
	condelseif->setup(globalNamespace);

	// Else if block
	auto varreadMyVariable1elseif = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &myVariable1);
	auto opsubtractelseif = std::make_unique<gscript::ScriptOperatorSubtractFrom>(globalNamespace, gscript::OperatorLinkage::Both);
	auto literal10elseif = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(5));

	auto stmtvecbodyelseif = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbodyelseif.push_back(std::move(varreadMyVariable1elseif));
	stmtvecbodyelseif.push_back(std::move(opsubtractelseif));
	stmtvecbodyelseif.push_back(std::move(literal10elseif));
	auto stmt1elseif = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbodyelseif));
	stmt1elseif->setup(globalNamespace);
	auto stmtvecbody1elseif = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmtvecbody1elseif.push_back(std::move(stmt1elseif));
	auto ebelseif = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1elseif));

	// Else if
	auto selseif = std::make_unique<gscript::ScriptIf>(globalNamespace, std::move(condelseif), std::move(selse));
	selseif->merge(std::move(ebelseif));

	// If
	gscript::ScriptIf f(globalNamespace, std::move(cond), std::move(selseif));
	f.merge(std::move(eb));

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 42);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 52);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 62);

	testVariable.setValue(std::make_unique<gscript::ScriptIntValue>(13));

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 620);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6200);

	testVariable.setValue(std::make_unique<gscript::ScriptIntValue>(42));

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6210);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6220);

	testVariable.setValue(std::make_unique<gscript::ScriptIntValue>(1337));

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6215);

	f.run(globalNamespace);
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6210);
}