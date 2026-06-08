#include "common.h"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/class.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/return.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

namespace
{
	std::unique_ptr<gscript::ScriptClass> createClass(gscript::ScriptNamespace& globalNamespace)
	{
		// Class
		auto myClass = std::make_unique<gscript::ScriptClass>(globalNamespace, "MyClass");
		myClass->registerVariable("test", gscript::ScriptType::create(gscript::ValueType::Int, *myClass), std::make_unique<gscript::ScriptIntValue>(42));

		// Operator+ function block
		{
			auto class_fnc = std::make_unique<gscript::ScriptMethod>(
				globalNamespace,
				"operator+",
				gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace),
				gscript::PARAMS_T{
					{ std::make_shared<gscript::ScriptClassType>(*myClass), "arg1" },
					{ std::make_shared<gscript::ScriptType>(gscript::ValueType::Int), "arg2" }
				},
				gscript::Modifier::AccessPublic | gscript::Modifier::Static
			);

			auto literal42 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(42));
			auto opadd = std::make_unique<gscript::ScriptOperatorAdd>(gscript::OperatorLinkage::Both);
			auto varreadParam1 = std::make_unique<gscript::ScriptVarReadPrototype>("arg2");
			auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
			stmtvecbody.push_back(std::move(literal42));
			stmtvecbody.push_back(std::move(opadd));
			stmtvecbody.push_back(std::move(varreadParam1));
			auto stmt1 = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
			stmt1->setup(*class_fnc);
			auto ret = std::make_unique<gscript::ScriptReturn>(std::move(stmt1));
			auto stmtvecbody1 = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
			stmtvecbody1.push_back(std::move(ret));
			auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

			class_fnc->merge(std::move(eb));

			myClass->registerFunction(std::move(class_fnc));
		}

		// Operator+= function block
		{
			auto class_fnc = std::make_unique<gscript::ScriptMethod>(
				globalNamespace,
				"operator+=",
				gscript::ScriptType::createClassReference(myClass.get()),
				gscript::PARAMS_T{
					{ std::make_shared<gscript::ScriptClassType>(*myClass), "arg1" },
					{ std::make_shared<gscript::ScriptType>(gscript::ValueType::Int), "arg2" }
				},
				gscript::Modifier::AccessPublic | gscript::Modifier::Static
			);

			// Modify variable
			auto varreadtest = std::make_unique<gscript::ScriptVarReferenceRead>(*myClass, "test");
			auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(gscript::OperatorLinkage::Both);
			auto varreadParam1 = std::make_unique<gscript::ScriptVarReadPrototype>("arg2");
			auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
			stmtvecbody.push_back(std::move(varreadtest));
			stmtvecbody.push_back(std::move(opadd));
			stmtvecbody.push_back(std::move(varreadParam1));
			auto stmt1 = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
			stmt1->setup(*class_fnc);
			// Read this
			auto varreadThis = std::make_unique<gscript::ScriptVarReadPrototype>("arg1");
			auto stmtretvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
			stmtretvecbody.push_back(std::move(varreadThis));
			// Return
			auto stmtret = std::make_unique<gscript::ScriptStatement>(std::move(stmtretvecbody));
			stmtret->setup(*class_fnc);
			auto ret = std::make_unique<gscript::ScriptReturn>(std::move(stmtret));
			// Statement
			auto stmtvecbody1 = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
			stmtvecbody1.push_back(std::move(stmt1));
			stmtvecbody1.push_back(std::move(ret));
			auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

			class_fnc->merge(std::move(eb));

			myClass->registerFunction(std::move(class_fnc));
		}

		return myClass;
	}
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Operator::BasicTest")
{
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(21)));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(gscript::OperatorLinkage::Both));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(21)));

	auto stmt = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
	stmt->setup(globalNamespace);
	auto result = stmt->run(globalNamespace);

	REQUIRE(result->getType()->getTypeDescriptor() == gscript::ValueType::Int);
	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Operator::TypeSelectionTest")
{
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptFloatValue>(13.37f)));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(gscript::OperatorLinkage::Both));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(21)));

	auto stmt = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
	stmt->setup(globalNamespace);
	auto result = stmt->run(globalNamespace);

	REQUIRE(result->getType()->getTypeDescriptor() == gscript::ValueType::Float);
	REQUIRE(result->as<gscript::ScriptFloatValue>().getValue() == 34.37f);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Operator::BoolTypeSelectionTest")
{
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(42)));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorEquals>(gscript::OperatorLinkage::Both));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(1337)));

	auto stmt = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
	stmt->setup(globalNamespace);
	auto result = stmt->run(globalNamespace);

	REQUIRE(result->getType()->getTypeDescriptor() == gscript::ValueType::Bool);
	REQUIRE(result->as<gscript::ScriptBoolValue>().getValue() == false);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Operator::MemberOperator")
{
	auto cls = createClass(globalNamespace);
	auto inst = cls->instantiate();

	gscript::ScriptClassValue instValue(std::move(inst), *cls);

	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptReferenceValue>(&instValue)));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(gscript::OperatorLinkage::Both));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(1337)));

	auto stmt = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
	stmt->setup(globalNamespace);
	auto result = stmt->run(globalNamespace);

	REQUIRE(result->getType()->getTypeDescriptor() == gscript::ValueType::Int);
	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 1337 + 42);
}

TEST_CASE_METHOD(GscriptTest, "Runtime::Operator::MemberOperatorModifying")
{
	auto cls = createClass(globalNamespace);
	auto inst = cls->instantiate();

	gscript::ScriptClassValue instValue(std::move(inst), *cls);

	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptReferenceValue>(&instValue)));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAddTo>(gscript::OperatorLinkage::Both));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(1337)));

	auto stmt = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
	stmt->setup(globalNamespace);
	auto result = stmt->run(globalNamespace);
	auto retcls = result->as<gscript::ScriptClassValue>();
	REQUIRE(result->getType()->getTypeDescriptor() == gscript::ValueType::Reference);
	REQUIRE(std::static_pointer_cast<gscript::ScriptReferenceType>(result->getType())->getUnderlyingTypeDescriptor() == gscript::ValueType::Class);
	
	//REQUIRE(result->data()->as<gscript::ScriptClassValue>().getValue()->findVariable("test")->getValue()->as<gscript::ScriptIntValue>().getValue() == 1337 + 42);
	REQUIRE(result->data()->as<gscript::ScriptClassValue>().getValue()->getClass().findVariable("test")->getValue()->as<gscript::ScriptIntValue>().getValue() == 1337 + 42);
}