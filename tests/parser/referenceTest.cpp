#include "common.h"
#include "gscript/parser/pReference.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserReferenceSimple")
{
	std::string txt = "&";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserReferenceWithName")
{
	std::string txt = "&something";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserReferenceFailure")
{
	std::string txt = "something";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"&\", got \"s\"");
}

TEST_CASE("ParserReferenceFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"&\", got empty string");
}

TEST_CASE("ParserReferenceCommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"&";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserReferenceCommentLineAfter")
{
	std::string txt = 
		"&\n"
		"// This is a comment";

	gscript::ParserReference pRef;
	auto result = pRef.parse(txt);

	REQUIRE(result.isOk());
}