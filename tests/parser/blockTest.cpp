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
	REQUIRE(result.details.message == "Expected \"}\", got \"a\"");
}