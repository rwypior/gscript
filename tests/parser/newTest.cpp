#include "common.h"
#include "gscript/parser/pNew.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserNewSimple")
{
	std::string txt = "new someclass();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNew.name == "someclass");
}

TEST_CASE("ParserNewNamespaceAccessor")
{
	std::string txt = "new ns::someclass();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNew.name == "ns::someclass");
}

TEST_CASE("ParserNewFailureNoMethod")
{
	std::string txt = "new();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserNewFailureNoParentheses")
{
	std::string txt = "new;";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserNewFailureOnlyNew")
{
	std::string txt = "new";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserNewFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"new\", got empty statement");
}

TEST_CASE("ParserNewCommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"new someclass();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNew.name == "someclass");
}

TEST_CASE("ParserNewCommentBlockAfterNew")
{
	std::string txt = "new /* This is a comment */ someclass();";

	gscript::ParserNew pNew;
	auto result = pNew.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNew.name == "someclass");
}