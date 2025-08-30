#include "common.h"
#include "gscript/parser/pInherit.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserInheritSimple")
{
	std::string txt = ":something";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserInheritSpaces")
{
	std::string txt = "			   :  	 something		";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserInheritFailure")
{
	std::string txt = "  just some stuff    ";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \":\", got \"j\"");
}

TEST_CASE("ParserInheritFailureEmpty")
{
	std::string txt = "";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \":\", got empty string");
}

TEST_CASE("ParserInheritFailureNoTarget")
{
	std::string txt = ":";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}