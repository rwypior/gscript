#include "common.h"
#include "gscript/parser/pNew.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::New::Simple")
{
	std::string txt = "new someclass();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNew.name == "someclass");
}

TEST_CASE("Parser::New::NamespaceAccessor")
{
	std::string txt = "new ns::someclass();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNew.name == "ns::someclass");
}

TEST_CASE("Parser::New::FailureNoMethod")
{
	std::string txt = "new();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::New::FailureNoParentheses")
{
	std::string txt = "new;";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::New::FailureOnlyNew")
{
	std::string txt = "new";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::New::FailureEmptyString")
{
	std::string txt = "";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"new\", got empty statement");
}

TEST_CASE("Parser::New::CommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"new someclass();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNew.name == "someclass");
}

TEST_CASE("Parser::New::CommentBlockAfterNew")
{
	std::string txt = "new /* This is a comment */ someclass();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNew.name == "someclass");
}