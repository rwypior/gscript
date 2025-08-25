#include "common.h"
#include "gscript/parser/pFor.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserForSimple")
{
	std::string txt = "for (int i; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForWithBody")
{
	std::string txt = 
		"for (int i; i < 5; i++)\n"
		"{\n"
		"somevar += i;\n"
		"}\n";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForFailureNoArgList")
{
	std::string txt = "for";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserForFailureNoBody")
{
	std::string txt = "for(int i; i < 10; i++)\n";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(!result.isOk());
}