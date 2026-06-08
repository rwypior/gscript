#include "common.h"
#include "gscript/parser/pSpecialDirective.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Special::Simple")
{
	std::string txt = "@test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Special::WithLeadingSpaces")
{
	std::string txt = "     @test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	auto dupsko = result.result.end - txt.begin();

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.begin() + 5 + 5);
}

TEST_CASE("Parser::Special::WithTrailingSpaces")
{
	std::string txt = "@test     ";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 5);
}

TEST_CASE("Parser::Special::WithTrailingLeadingSpaces")
{
	std::string txt = "     @test     ";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.begin() + 5 + 5);
}

TEST_CASE("Parser::Special::DirectiveFailureNoChar")
{
	std::string txt = "test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::Special::DirectiveFailureWrongChar")
{
	std::string txt = "#test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::Special::DirectiveFailureWrongDirective")
{
	std::string txt = "#something";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::Special::DirectiveCommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"@test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 21);
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Special::DirectiveCommentBlockBefore")
{
	std::string txt = 
		"/* This is a comment */\n"
		"@test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 24);
	REQUIRE(result.result.end == txt.end());
}