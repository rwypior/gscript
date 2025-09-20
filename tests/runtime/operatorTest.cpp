#include "common.h"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "RuntimeOperatorBasicTest")
{
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(21)));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(21)));

	auto stmt = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	stmt->setup();
	auto result = stmt->run();

	REQUIRE(result->getType()->getTypeDescriptor() == gscript::VALUE_TYPE_T::VT_INT);
	REQUIRE(result->as<gscript::ScriptIntValue>().getValue() == 42);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeOperatorTypeSelectionTest")
{
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptFloatValue>(13.37f)));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorAdd>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(21)));

	auto stmt = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	stmt->setup();
	auto result = stmt->run();

	REQUIRE(result->getType()->getTypeDescriptor() == gscript::VALUE_TYPE_T::VT_FLOAT);
	REQUIRE(result->as<gscript::ScriptFloatValue>().getValue() == 34.37f);
}

TEST_CASE_METHOD(GscriptTest, "RuntimeOperatorBoolTypeSelectionTest")
{
	auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(42)));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptOperatorEquals>(globalNamespace, gscript::OPERATOR_LINK_T::OL_BOTH));
	stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(1337)));

	auto stmt = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(stmtvecbody));
	stmt->setup();
	auto result = stmt->run();

	REQUIRE(result->getType()->getTypeDescriptor() == gscript::VALUE_TYPE_T::VT_BOOL);
	REQUIRE(result->as<gscript::ScriptBoolValue>().getValue() == false);
}