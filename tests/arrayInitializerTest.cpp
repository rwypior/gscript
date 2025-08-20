#include <catch2/catch_all.hpp>

#include "common.h"
#include "gscript/arrayInitializer.hpp"
#include "gscript/pArrayInitializer.hpp"
#include "gscript/scriptValue.hpp"

TEST_CASE_METHOD(GscriptTest, "Array initializer")
{
	std::string src = "{1, 2, 3, 4};";

	gscript::ParserArrayInitializer pai;
	auto result = pai.parse({ src.begin(), src.end() });

	REQUIRE(result.isOk());

	gscript::ScriptArrayInitializer ai(globalNamespace, pai);
	
	REQUIRE(ai.getType()->getTypeDescriptor() == gscript::VALUE_TYPE_T::VT_ARRAY);

	//auto* av = static_cast<gscript::ScriptArrayValue*>(ai.run());
	// TODO - add testing of subelements
}