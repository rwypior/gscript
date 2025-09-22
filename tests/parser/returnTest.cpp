#include "common.h"
#include "gscript/parser/pReturn.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pFuncCall.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserReturnSimple")
{
	std::string txt = "return 42;";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pReturn.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pReturn.value.components.at(0))->value == "42");
}

TEST_CASE("ParserReturnFuncCall")
{
	std::string txt = "return somefunc();";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pReturn.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pReturn.value.components.at(0))->name == "somefunc");
}

TEST_CASE("ParserReturnFailureNothing")
{
	std::string txt = "return";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Empty statement");
}

TEST_CASE("ParserReturnFailureNoSemicolon")
{
	std::string txt = "return 13";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got empty string");
}

TEST_CASE("ParserReturnFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"return\", got empty statement");
}

TEST_CASE("ParserReturnCommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"return 42;";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pReturn.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pReturn.value.components.at(0))->value == "42");
}

TEST_CASE("ParserReturnCommentBlockAfterReturn")
{
	std::string txt = "return /* This is a comment */ 42;";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pReturn.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pReturn.value.components.at(0))->value == "42");
}