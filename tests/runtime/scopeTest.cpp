#include "common.h"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/return.hpp"
#include "gscript/runtime/if.hpp"
#include "gscript/runtime/funcCall.hpp"
#include "gscript/runtime/varDeclaration.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "Runtime::Scope::VariableOutsideScope")
{
	// Function
	gscript::ScriptFunction myFunc(globalNamespace, "myfunc", gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace), {}, {});

	// Variable

	auto literal42 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(42));
	auto literal42vec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	literal42vec.push_back(std::move(literal42));

	auto varstmt = std::make_unique<gscript::ScriptStatement>(std::move(literal42vec));
	varstmt->setup(myFunc);

	auto& sv = myFunc.registerVariable("myVariable1", gscript::ScriptType::create(gscript::ValueType::Int, myFunc), std::make_unique<gscript::ScriptIntValue>(0));
	auto vd = std::make_unique<gscript::ScriptVarDeclaration>(myFunc, sv, std::move(varstmt));

	// If
	auto iftrue = std::make_unique<gscript::ScriptLiteral>(gscript::ScriptType::btrue());
	std::vector<std::unique_ptr<gscript::ScriptCallable>> ifcallables;
	ifcallables.push_back(std::move(iftrue));
	auto ifstmt = std::make_unique<gscript::ScriptStatement>(std::move(ifcallables));
	ifstmt->setup(myFunc);

	// If block

	auto sif = std::make_unique<gscript::ScriptIf>(myFunc, std::move(ifstmt), nullptr);

	auto varreadMyVariable1 = std::make_unique<gscript::ScriptVarRead>(*sif, &sv);
	std::vector<std::unique_ptr<gscript::ScriptCallable>> ifblockcallables;
	ifblockcallables.push_back(std::move(varreadMyVariable1));
	auto ifblockstmt = std::make_unique<gscript::ScriptStatement>(std::move(ifblockcallables));
	ifblockstmt->setup(*sif);
	auto ifblockreturn = std::make_unique<gscript::ScriptReturn>(std::move(ifblockstmt));
	ifblockreturn->setup(*sif);
	std::vector<std::unique_ptr<gscript::ScriptCallable>> ifblockstatements;
	ifblockstatements.push_back(std::move(ifblockreturn));

	sif->merge(std::move(ifblockstatements));
	sif->setup(myFunc);

	// Return
	auto literal1337 = std::make_unique<gscript::ScriptLiteral>(std::make_unique<gscript::ScriptIntValue>(42));
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::move(literal1337));
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
	stmt1->setup(myFunc);
	
	auto ret = std::make_unique<gscript::ScriptReturn>(std::move(stmt1));

	auto stmtvecbodycallable = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbodycallable.push_back(std::move(vd));
	stmtvecbodycallable.push_back(std::move(sif));
	stmtvecbodycallable.push_back(std::move(ret));
	auto eb = std::make_unique<gscript::ScriptExecutiveBlock>(std::move(stmtvecbodycallable));

	myFunc.merge(std::move(eb));

	auto result = myFunc.run(myFunc);
	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 42);

	result = myFunc.run(myFunc);
	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 42);

	result = myFunc.run(myFunc);
	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 42);
}