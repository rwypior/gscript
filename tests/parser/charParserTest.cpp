#include "common.h"
#include "gscript/parser/pChar.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Char::Simple")
{
	std::string txt = "x";
	auto result = gscript::ParserChar::parse(txt, 'x');

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Char::Spaces")
{
	std::string txt = "     x";
	auto result = gscript::ParserChar::parse(txt, 'x');

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Char::TrailingSpaces")
{
	std::string txt = "x     ";
	auto result = gscript::ParserChar::parse(txt, 'x');

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 1);
}

TEST_CASE("Parser::Char::TrailingLeadingSpaces")
{
	std::string txt = "     x     ";
	auto result = gscript::ParserChar::parse(txt, 'x');

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.begin() + 5 + 1);
}

TEST_CASE("Parser::Char::SpacesNotAllowedFailure")
{
	std::string txt = "     x";
	auto result = gscript::ParserChar::parse(txt, 'x', false);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::Char::SpacesNotAllowed")
{
	std::string txt = "x";
	auto result = gscript::ParserChar::parse(txt, 'x', false);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Char::SpacesNotAllowedWithTrailing")
{
	std::string txt = "x     ";
	auto result = gscript::ParserChar::parse(txt, 'x', false);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 1);
}

TEST_CASE("Parser::Char::ParseUntilNonWhitespaceSimple")
{
	std::string txt = "     x";
	auto result = gscript::ParserChar::parseUntilNonWhitespace(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Char::ParseUntilNonWhitespaceTrailingSpaces")
{
	std::string txt = "     x   ";
	auto result = gscript::ParserChar::parseUntilNonWhitespace(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.end() - 3);
}

TEST_CASE("Parser::Char::ParseUntilNonWhitespaceNewlines")
{
	std::string txt = 
		"     \n"
		"   x";
	auto result = gscript::ParserChar::parseUntilNonWhitespace(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 9);
	REQUIRE(result.result.end == txt.end());
}