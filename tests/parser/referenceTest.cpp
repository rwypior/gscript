#include "common.h"
#include "gscript/parser/pReference.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Reference::Simple")
{
	std::string txt = "&";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Reference::WithName")
{
	std::string txt = "&something";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Reference::Failure")
{
	std::string txt = "something";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"&\", got \"s\"");
}

TEST_CASE("Parser::Reference::FailureEmptyString")
{
	std::string txt = "";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"&\", got empty string");
}

TEST_CASE("Parser::Reference::CommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"&";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Reference::CommentLineAfter")
{
	std::string txt = 
		"&\n"
		"// This is a comment";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(result.isOk());
}