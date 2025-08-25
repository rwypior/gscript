#include "common.h"
#include "gscript/parser/pWord.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParseWord")
{
	std::string txt = "word";
	auto result = gscript::ParserWord::parse(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("ParseWordPrependedWhitechars")
{
	std::string txt = "			word";
	auto result = gscript::ParserWord::parse(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 3);
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("ParseWordAppendedWhitechars")
{
	std::string txt = "word				";
	auto result = gscript::ParserWord::parse(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 4);
}

TEST_CASE("ParseWordAppendedPrependedWhitechars")
{
	std::string txt = "			word				";
	auto result = gscript::ParserWord::parse(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 3);
	REQUIRE(result.result.end == txt.begin() + 7);
}

TEST_CASE("ParseUntil")
{
	std::string txt = "blablaword";
	auto result = gscript::ParserWord::parseUntil(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 6);
}