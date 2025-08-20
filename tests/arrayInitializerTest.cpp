#include "common.h"
#include "gscript/runtime/arrayInitializer.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/callable.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "Array initializer")
{
	auto literal42 = std::make_unique<gscript::ScriptLiteral>(globalNamespace, std::make_unique<gscript::ScriptIntValue>(42));
	auto literal42vec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	literal42vec.push_back(std::move(literal42));
	
	auto stmt1 = std::make_unique<gscript::ScriptStatement>(globalNamespace, std::move(literal42vec));

	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptStatement>>();
	stmtvec.push_back(std::move(stmt1));

	gscript::ScriptArrayInitializer ai(globalNamespace, std::move(stmtvec));

	// Array's type is the underlying element type
	REQUIRE(ai.getType()->getTypeDescriptor() == gscript::VALUE_TYPE_T::VT_INT);

	auto val = ai.run();

	REQUIRE(val->getType()->getTypeDescriptor() == gscript::VALUE_TYPE_T::VT_INT);

	auto arr = static_cast<gscript::ScriptArrayValue*>(val)->getValue();
	REQUIRE(static_cast<gscript::ScriptIntValue*>(arr[0])->getValue() == 42);
}