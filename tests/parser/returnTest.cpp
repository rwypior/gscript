#include "common.h"
#include "gscript/parser/pReturn.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pFuncCall.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Return::Simple")
{
	std::string txt = "return 42;";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pReturn.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pReturn.value.components.at(0))->value == "42");
}

TEST_CASE("Parser::Return::FuncCall")
{
	std::string txt = "return somefunc();";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pReturn.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pReturn.value.components.at(0))->name == "somefunc");
}

TEST_CASE("Parser::Return::FailureNothing")
{
	std::string txt = "return";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Empty statement");
}

TEST_CASE("Parser::Return::FailureNoSemicolon")
{
	std::string txt = "return 13";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got empty string");
}

TEST_CASE("Parser::Return::FailureEmptyString")
{
	std::string txt = "";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"return\", got empty statement");
}

TEST_CASE("Parser::Return::CommentLineBefore")
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

TEST_CASE("Parser::Return::CommentLineBeforeTwice")
{
	std::string txt = 
		"// This is a comment\n"
		"// And another one\n"
		"return 42;";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pReturn.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pReturn.value.components.at(0))->value == "42");
}

TEST_CASE("Parser::Return::CommentBlockAfterReturn")
{
	std::string txt = "return /* This is a comment */ 42;";

	gscript::ParserReturn pReturn;
	auto result = pReturn.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pReturn.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pReturn.value.components.at(0))->value == "42");
}