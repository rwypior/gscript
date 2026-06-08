#include "common.h"
#include "gscript/parser/pInherit.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Inherit::Simple")
{
	std::string txt = ":something";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Inherit::Spaces")
{
	std::string txt = "			   :  	 something		";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Inherit::Failure")
{
	std::string txt = "  just some stuff    ";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \":\", got \"j\"");
}

TEST_CASE("Parser::Inherit::FailureEmpty")
{
	std::string txt = "";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \":\", got empty string");
}

TEST_CASE("Parser::Inherit::FailureNoTarget")
{
	std::string txt = ":";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::Inherit::CommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		":something";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Inherit::CommentBlockBefore")
{
	std::string txt = "/* This is a comment */:something";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Inherit::CommentBlockAfterColon")
{
	std::string txt = ": /* This is a comment */something";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Inherit::CommentLineAfterName")
{
	std::string txt = ": something // This is a comment";

	gscript::ParserInherit pInherit;
	auto result = pInherit.parse(txt);

	REQUIRE(result.isOk());
}