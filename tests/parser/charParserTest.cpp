#include "common.h"
#include "gscript/parser/pChar.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParseChar")
{
	std::string txt = "x";
	auto result = gscript::ParserChar::parse(txt, 'x');

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("ParseCharSpaces")
{
	std::string txt = "     x";
	auto result = gscript::ParserChar::parse(txt, 'x');

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("ParseCharTrailingSpaces")
{
	std::string txt = "x     ";
	auto result = gscript::ParserChar::parse(txt, 'x');

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 1);
}

TEST_CASE("ParseCharTrailingLeadingSpaces")
{
	std::string txt = "     x     ";
	auto result = gscript::ParserChar::parse(txt, 'x');

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.begin() + 5 + 1);
}

TEST_CASE("ParseCharSpacesNotAllowedFailure")
{
	std::string txt = "     x";
	auto result = gscript::ParserChar::parse(txt, 'x', false);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParseCharSpacesNotAllowed")
{
	std::string txt = "x";
	auto result = gscript::ParserChar::parse(txt, 'x', false);

	REQUIRE(result.isOk());
}

TEST_CASE("ParseCharSpacesNotAllowedWithTrailing")
{
	std::string txt = "x     ";
	auto result = gscript::ParserChar::parse(txt, 'x', false);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 1);
}