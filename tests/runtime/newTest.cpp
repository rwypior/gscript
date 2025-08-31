#include "common.h"
#include "gscript/runtime/method.hpp"
#include "gscript/runtime/new.hpp"
#include "gscript/runtime/class.hpp"
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

TEST_CASE_METHOD(GscriptTest, "RuntimeNewSimple")
{
	// Variable
	gscript::ScriptVariable myVariable1("myVariable1", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_INT, globalNamespace), new gscript::ScriptIntValue(1), 0);

	// Class
	gscript::ScriptClass myClass(globalNamespace, "MyClass");

	// Function
	auto myConstructor = std::make_unique<gscript::ScriptMethod>(globalNamespace, "MyClass", gscript::ScriptType::create(gscript::VALUE_TYPE_T::VT_VOID, globalNamespace));

	// Function block

	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(*myConstructor, &myVariable1);
	auto opadd = std::make_unique<gscript::ScriptOperatorAddTo>(*myConstructor, gscript::OPERATOR_LINK_T::OL_BOTH);
	auto literal5 = std::make_unique<gscript::ScriptLiteral>(*myConstructor, std::make_unique<gscript::ScriptIntValue>(5));
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(varreadMyVariable1));
	stmtvecbody.push_back(std::move(opadd));
	stmtvecbody.push_back(std::move(literal5));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(*myConstructor, std::move(stmtvecbody));
	auto stmtvecbody1 = std::vector<std::shared_ptr<gscript::ScriptCallable>>();
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