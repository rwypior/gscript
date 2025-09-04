#include "common.h"
#include "gscript/runtime/class.hpp"
#include "gscript/runtime/new.hpp"
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

TEST_CASE_METHOD(GscriptTest, "RuntimeClassNewTest")
{
	// Variable
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(1));

	// Class
	gscript::ScriptClass myClass(globalNamespace, "MyClass");

	// Constructor
	auto myConstructor = std::make_unique<gscript::ScriptMethod>(globalNamespace, "MyClass", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_VOID, globalNamespace));

	// Constructor block
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(*myConstructor, &myVariable1);
	auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(*myConstructor, gscript::OPERATOR_LINK_T::OL_BOTH);
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
	myClass.registerVariable("test", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, myClass), new gscript::ScriptIntValue(42));

	// New
	gscript::ScriptNew newcall(myClass);
	auto myClassObject = newcall.run();

	auto& myClassObjectVar = globalNamespace.registerVariable("myClassObject", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_CLASS, globalNamespace, "MyClass"), myClassObject);

	// Variable read
	auto varreadobject = std::make_unique<gscript::ScriptVarRead>(globalNamespace, "myClassObject");
	auto memberaccess = std::make_unique<gscript::ScriptOperatorMemberAccessor>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH);
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
	auto& myVariable1 = globalNamespace.registerVariable("myVariable1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(1));

	// Base Class
	gscript::ScriptClass base(globalNamespace, "Base");

	// Class
	gscript::ScriptClass myClass(globalNamespace, "MyClass", &base);

	// Base function
	auto base_fnc = std::make_unique<gscript::ScriptMethod>(globalNamespace, "fnc", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_VOID, globalNamespace));

	// Constructor block
	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(*base_fnc, &myVariable1);
	auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(*base_fnc, gscript::OPERATOR_LINK_T::OL_BOTH);
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

	auto objval = static_cast<gscript::ScriptClassValue*>(newcall.run());
	auto obj = objval->getValue();

	auto foundfnc = obj->findFunction("fnc", {});

	REQUIRE(foundfnc);
	REQUIRE(foundfnc->getName() == "fnc");
}