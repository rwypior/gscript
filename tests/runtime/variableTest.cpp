#include "common.h"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "Runtime::Variable::BasicRead")
{
	auto& sv = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42));

	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, "myVariable1");

	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));

	auto stmt1 = std::make_unique<gscript::ScriptStatement>(std::move(stmtvec));
	stmt1->setup(globalNamespace);

	auto result = stmt1->run(globalNamespace);

	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Variable::ArrayRead")
{
	std::vector<std::unique_ptr<gscript::ScriptValue>> entries;
	entries.push_back(std::make_unique<gscript::ScriptIntValue>(1));
	entries.push_back(std::make_unique<gscript::ScriptIntValue>(2));
	entries.push_back(std::make_unique<gscript::ScriptIntValue>(3));
	entries.push_back(std::make_unique<gscript::ScriptIntValue>(4));
	entries.push_back(std::make_unique<gscript::ScriptIntValue>(5));

	auto arr = std::make_unique<gscript::ScriptArrayValue>();
	arr->setValue(entries);
	
	auto& sv = globalNamespace.registerVariable(
		"myArray1", 
		gscript::ScriptType::create(gscript::TypeDescriptor(gscript::ValueType::Array, gscript::ValueType::Int), globalNamespace),
		std::move(arr)
	);

	auto literal0 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(0));
	auto varread0 = std::make_unique<gscript::ScriptArrayRead>(globalNamespace, "myArray1", std::move(literal0));
	auto result0 = varread0->run(globalNamespace);
	REQUIRE(result0->as<gscript::ScriptIntValue>().getValue() == 1);
	
	auto literal4 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(4));
	auto varread4 = std::make_unique<gscript::ScriptArrayRead>(globalNamespace, "myArray1", std::move(literal4));
	auto result4 = varread4->run(globalNamespace);
	REQUIRE(result4->as<gscript::ScriptIntValue>().getValue() == 5);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Variable::ReadFromNamespace")
{
	auto& first = globalNamespace.registerNamespace(std::make_unique<gscript::ScriptNamespace>(&globalNamespace, "first"));
	auto& second = globalNamespace.registerNamespace(std::make_unique<gscript::ScriptNamespace>(&globalNamespace, "second"));

	auto& something = first.registerVariable("something", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42));
	auto& blabla = second.registerVariable("blabla", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1337));

	auto varreadsomething = std::make_unique<gscript::ScriptVarRead>(first, "something");
	auto stmtvecsomething = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecsomething.push_back(std::move(varreadsomething));
	auto stmtsomething = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecsomething));
	stmtsomething->setup(globalNamespace);

	auto varreadblabla = std::make_unique<gscript::ScriptVarRead>(second, "blabla");
	auto stmtvecblabla = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecblabla.push_back(std::move(varreadblabla));
	auto stmtblabla = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecblabla));
	stmtblabla->setup(globalNamespace);

	auto resultsomething = stmtsomething->run(first);
	REQUIRE(resultsomething->as<gscript::ScriptIntValue>().getValue() == 42);

	auto resultblabla = stmtblabla->run(second);
	REQUIRE(resultblabla->as<gscript::ScriptIntValue>().getValue() == 1337);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Variable::Declaration")
{
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(42));
	auto literal42vec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	literal42vec.push_back(std::move(literal42));

	auto stmt1 = std::make_unique<gscript::ScriptStatement>(std::move(literal42vec));
	stmt1->setup(globalNamespace);

	auto& sv = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(0));
	gscript::ScriptVarDeclaration vd(globalNamespace, sv, std::move(stmt1));

	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 0);

	vd.run(globalNamespace);

	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Variable::Registering")
{
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(42));
	auto literal42vec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	literal42vec.push_back(std::move(literal42));

	auto stmt1 = std::make_unique<gscript::ScriptStatement>(std::move(literal42vec));
	stmt1->setup(globalNamespace);

	auto sv = std::make_unique<gscript::ScriptVariable>("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(0));

	auto myVariable1 = globalNamespace.findVariable("myVariable1");
	REQUIRE(myVariable1 == nullptr);
	REQUIRE(sv);

	globalNamespace.registerVariable(std::move(sv));

	myVariable1 = globalNamespace.findVariable("myVariable1");
	REQUIRE(myVariable1 != nullptr);

	REQUIRE(myVariable1->getValue()->as<gscript::ScriptIntValue>().getValue() == 0);

	gscript::ScriptVarDeclaration vd(globalNamespace, *myVariable1, std::move(stmt1));
	vd.run(globalNamespace);

	REQUIRE(myVariable1->getValue()->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Variable::Modifying")
{
	auto& sv = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(0));

	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, &sv);
	auto assign = std::make_unique<gscript::ScriptOperatorAssign>(gscript::OperatorLinkage::Both);
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(42));

	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));
	stmtvec.push_back(std::move(assign));
	stmtvec.push_back(std::move(literal42));

	auto stmt1 = std::make_unique<gscript::ScriptStatement>(std::move(stmtvec));
	stmt1->setup(globalNamespace);
	
	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 0);

	stmt1->run(globalNamespace);

	REQUIRE(sv.getValue()->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Variable::FailureNotFound")
{
	auto& sv = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(42));

	REQUIRE_THROWS_MATCHES(
		std::make_unique<gscript::ScriptVarRead>(globalNamespace, "notexisting"), 
		gscript::VariableNotFoundException,
		Catch::Matchers::Message("Variable \"notexisting\" was not found"));
}