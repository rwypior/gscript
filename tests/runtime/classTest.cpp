#include "common.h"
#include "gscript/runtime/class.hpp"
#include "gscript/runtime/classInstance.hpp"
#include "gscript/runtime/new.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/funcCall.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "RuntimeClassNewTest")
{
	// Variable
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1));

	// Class
	gscript::ScriptClass myClass(globalNamespace, "MyClass");

	// Constructor
	auto myConstructor = std::make_unique<gscript::ScriptMethod>(globalNamespace, "MyClass", gscript::ScriptType::create(gscript::ValueType::Void, globalNamespace));

	// Constructor block
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(*myConstructor, &myVariable1);
	auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(*myConstructor, gscript::OperatorLinkage::Both);
	auto literal5 = std::make_unique<gscript::ScriptLiteral>(*myConstructor, std::make_unique<gscript::ScriptIntValue>(5));
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(opadd));
	stmtvecbody.push_back(std::move(literal5));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(*myConstructor, std::move(stmtvecbody));
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmt1->setup();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	myConstructor->merge(std::move(eb));

	myClass.registerFunction(std::move(myConstructor));

	// New
	gscript::ScriptNew newcall(myClass);

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 1);

	newcall.run();
	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeClassVariableRead")
{
	// Class
	auto& myClass = globalNamespace.registerClass(std::make_unique<gscript::ScriptClass>(globalNamespace, "MyClass"));
	myClass.registerVariable("test", gscript::ScriptType::create(gscript::ValueType::Int, myClass), std::make_unique<gscript::ScriptIntValue>(42));

	// New
	gscript::ScriptNew newcall(myClass);
	auto myClassObject = newcall.run();

	auto& myClassObjectVar = globalNamespace.registerVariable("myClassObject", gscript::ScriptType::create(gscript::ValueType::Class, globalNamespace, "MyClass"), myClassObject);

	// Variable read
	auto varreadobject = std::make_unique<gscript::ScriptVarRead>(globalNamespace, "myClassObject");
	auto memberaccess = std::make_unique<gscript::ScriptOperatorMemberAccessor>(globalNamespace, gscript::OperatorLinkage::Both);
	auto varreadtest = std::make_unique<gscript::ScriptVarRead>(myClass, "test");

	auto stmtvectest = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvectest.push_back(std::move(varreadobject));
	stmtvectest.push_back(std::move(memberaccess));
	stmtvectest.push_back(std::move(varreadtest));
	auto stmttest = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvectest));
	stmttest->setup();

	auto resultsomething = stmttest->run();
	REQUIRE(resultsomething->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeClassInheritance")
{
	// Variable
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1));

	// Base Class
	gscript::ScriptClass base(globalNamespace, "Base");

	// Class
	gscript::ScriptClass myClass(globalNamespace, "MyClass", &base);

	// Base function
	auto base_fnc = std::make_unique<gscript::ScriptMethod>(globalNamespace, "fnc", gscript::ScriptType::create(gscript::ValueType::Void, globalNamespace));

	// Constructor block
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(*base_fnc, &myVariable1);
	auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(*base_fnc, gscript::OperatorLinkage::Both);
	auto literal5 = std::make_unique<gscript::ScriptLiteral>(*base_fnc, std::make_unique<gscript::ScriptIntValue>(5));
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(opadd));
	stmtvecbody.push_back(std::move(literal5));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(*base_fnc, std::move(stmtvecbody));
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmt1->setup();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	base_fnc->merge(std::move(eb));

	base.registerFunction(std::move(base_fnc));

	// New
	gscript::ScriptNew newcall(myClass);

	auto res = newcall.run();
	auto objval = static_cast<gscript::ScriptClassValue*>(res->data());
	auto& obj = objval->getValue();

	auto foundfnc = obj->findFunction("fnc", {});

	REQUIRE(foundfnc);
	REQUIRE(foundfnc->getName() == "fnc");

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 1);

	foundfnc->run();

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeClassVirtualCall")
{
	// Variable
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), std::make_unique<gscript::ScriptIntValue>(1));

	// Base Class
	auto& base = globalNamespace.registerClass(std::make_unique<gscript::ScriptClass>(globalNamespace, "Base"));

	// Class
	gscript::ScriptClass myClass(globalNamespace, "MyClass", &base);

	// Base function
	auto base_fnc = std::make_unique<gscript::ScriptMethod>(
		globalNamespace,
		"fnc", 
		gscript::ScriptType::create(gscript::ValueType::Void, globalNamespace),
		gscript::PARAMS_T(),
		gscript::Modifier::AccessPublic | gscript::Modifier::Virtual);

	base.registerFunction(std::move(base_fnc));

	// Class function
	auto class_fnc = std::make_unique<gscript::ScriptMethod>(
		globalNamespace, 
		"fnc", 
		gscript::ScriptType::create(gscript::ValueType::Void, globalNamespace),
		gscript::PARAMS_T(),
		gscript::Modifier::AccessPublic
	);

	// Constructor block
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(*class_fnc, &myVariable1);
	auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(*class_fnc, gscript::OperatorLinkage::Both);
	auto literal5 = std::make_unique<gscript::ScriptLiteral>(*class_fnc, std::make_unique<gscript::ScriptIntValue>(5));
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(opadd));
	stmtvecbody.push_back(std::move(literal5));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(*class_fnc, std::move(stmtvecbody));
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
	stmt1->setup();
	stmtvecbody1.push_back(std::move(stmt1));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbody1));

	class_fnc->merge(std::move(eb));

	myClass.registerFunction(std::move(class_fnc));

	// New
	gscript::ScriptNew newcall(myClass);
	auto myClassObject = newcall.run();

	auto& myClassObjectVar = globalNamespace.registerVariable("myClassObject", gscript::ScriptType::create(gscript::ValueType::Class, globalNamespace, "Base"), myClassObject);

	auto varreadobject = std::make_unique<gscript::ScriptVarRead>(globalNamespace, "myClassObject");
	auto memberaccess = std::make_unique<gscript::ScriptOperatorMemberAccessor>(globalNamespace, gscript::OperatorLinkage::Both);
	auto varreadtest = std::make_unique<gscript::ScriptFuncCall>(myClass, "fnc");

	auto stmtvectest = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvectest.push_back(std::move(varreadobject));
	stmtvectest.push_back(std::move(memberaccess));
	stmtvectest.push_back(std::move(varreadtest));
	auto stmttest = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvectest));
	stmttest->setup();

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 1);

	auto resultsomething = stmttest->run();

	REQUIRE(myVariable1.getValue()->as<gscript::ScriptIntValue>().getValue() == 6);
}