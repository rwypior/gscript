#include "common.h"
#include "gscript/parser/pBlock.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserBlockSimple")
{
	std::string txt = 
		"{\n"
		"a_statement;\n"
		"}"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserBlockOneLiner")
{
	{
		std::string txt =
			"a_statement;";

		gscript::ParserBlock pBlock;
		auto result = pBlock.parse(txt);

		REQUIRE(result.isOk());
	}

	{
		std::string txt =
			"			    a_statement;		";

		gscript::ParserBlock pBlock;
		auto result = pBlock.parse(txt);

		REQUIRE(result.isOk());
	}
}

TEST_CASE("ParserBlockOneLinerNoSemicolon")
{
	std::string txt = 
		"		a_statement"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got empty string");
}

TEST_CASE("ParserBlockFailureNoSemicolon")
{
	std::string txt = 
		"{\n"
		"a_statement\n"
		"}"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got \"}\"");
}

TEST_CASE("ParserBlockFailureNoEnclosure")
{
	std::string txt = 
		"{\n"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"}\", got empty string");
}

TEST_CASE("ParserBlockFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected statement");
}