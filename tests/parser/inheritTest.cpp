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

TEST_CASE("ParserInheritCommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		":something";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserInheritCommentBlockBefore")
{
	std::string txt = "/* This is a comment */:something";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserInheritCommentBlockAfterColon")
{
	std::string txt = ": /* This is a comment */something";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserInheritCommentLineAfterName")
{
	std::string txt = ": something // This is a comment";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}