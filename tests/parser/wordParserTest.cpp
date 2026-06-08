#include "common.h"
#include "gscript/parser/pWord.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Word::Simple")
{
	std::string txt = "word";
	auto result = gscript::ParserWord::parse(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Word::PrependedWhitechars")
{
	std::string txt = "			word";
	auto result = gscript::ParserWord::parse(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 3);
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Word::AppendedWhitechars")
{
	std::string txt = "word				";
	auto result = gscript::ParserWord::parse(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 4);
}

TEST_CASE("Parser::Word::AppendedPrependedWhitechars")
{
	std::string txt = "			word				";
	auto result = gscript::ParserWord::parse(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 3);
	REQUIRE(result.result.end == txt.begin() + 7);
}

TEST_CASE("Parser::Word::ParseUntil")
{
	std::string txt = "blablaword";
	auto result = gscript::ParserWord::parseUntil(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 6);
}

TEST_CASE("Parser::Word::ParseUntilAtBegin")
{
	std::string txt = "wordblabla";
	auto result = gscript::ParserWord::parseUntil(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin());
}

TEST_CASE("Parser::Word::ParseUntilAtMiddle")
{
	std::string txt = "blawordbla";
	auto result = gscript::ParserWord::parseUntil(txt, "word");

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 3);
}

TEST_CASE("Parser::Word::ParseUntilFailure")
{
	std::string txt = "blablabla";
	auto result = gscript::ParserWord::parseUntil(txt, "word");

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::Word::ParseUntilWithAllowed")
{
	{
		std::string txt = "blablaword";
		auto result = gscript::ParserWord::parseUntil(txt, "word", nullptr, "bla");

		REQUIRE(result.isOk());
		REQUIRE(result.result.begin == txt.begin());
		REQUIRE(result.result.end == txt.begin() + 6);
	}
	
	{
		std::string txt = "testword";
		auto result = gscript::ParserWord::parseUntil(txt, "word", nullptr, "bla");

		REQUIRE(!result.isOk());
	}
}

TEST_CASE("Parser::Word::Empty")
{
	std::string txt = "";
	auto result = gscript::ParserWord::parseUntil(txt, "word");

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::Word::EmptyMultipleSpaces")
{
	std::string txt = "   ";
	auto result = gscript::ParserWord::parseUntil(txt, "word");

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::Word::Parse any character")
{
	{
		std::string txt = "asdf";
		auto result = gscript::ParserWord::parseAny(txt);

		REQUIRE(result.isOk());
		REQUIRE(result.result.begin == txt.begin());
		REQUIRE(result.result.end == txt.begin() + 4);
		REQUIRE(result.result.getWord() == "asdf");
	}
	
	{
		std::string txt = "  x  ";
		auto result = gscript::ParserWord::parseAny(txt);

		REQUIRE(result.isOk());
		REQUIRE(result.result.begin == txt.begin() + 2);
		REQUIRE(result.result.end == txt.begin() + 3);
		REQUIRE(result.result.getWord() == "x");
	}

	{
		std::string txt = "   1a+c/_5!  ";
		auto result = gscript::ParserWord::parseAny(txt);

		REQUIRE(result.isOk());
		REQUIRE(result.result.begin == txt.begin() + 3);
		REQUIRE(result.result.end == txt.begin() + 11);
		REQUIRE(result.result.getWord() == "1a+c/_5!");
	}
	
	{
		std::string txt = "   1a+c/{_5!  ";
		auto result = gscript::ParserWord::parseAny(txt);

		REQUIRE(result.isOk());
		REQUIRE(result.result.begin == txt.begin() + 3);
		REQUIRE(result.result.end == txt.begin() + 8);
		REQUIRE(result.result.getWord() == "1a+c/");
	}

	{
		std::string txt = "   {  ";
		auto result = gscript::ParserWord::parseAny(txt);

		REQUIRE(!result.isOk());
	}
}