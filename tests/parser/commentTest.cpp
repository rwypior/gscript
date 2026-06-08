#include "common.h"
#include "gscript/parser/pComment.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Comment::SingleLine")
{
	{
		std::string txt = "// This is a comment";

		gscript::ParserComment pComment;
		auto result = pComment.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(result.getLength() == txt.length());
		REQUIRE(result.result.end == txt.end());
	}

	{
		std::string txt = "//This is a comment";

		gscript::ParserComment pComment;
		auto result = pComment.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(result.getLength() == txt.length());
		REQUIRE(result.result.end == txt.end());
	}

	{
		std::string txt = "    // This is a comment";

		gscript::ParserComment pComment;
		auto result = pComment.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(result.getLength() == txt.length());
		REQUIRE(result.result.end == txt.end());
	}
}

TEST_CASE("Parser::Comment::LineWithNumbers")
{
	std::string txt = "// Th1s 1s 4 c0mm3nt";

	gscript::ParserComment pComment;
	auto result = pComment.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.getLength() == txt.length());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Comment::LineMultiple")
{
	std::string txt =
		"// This is a comment\n"
		"// Another comment";

	gscript::ParserComment pComment;
	auto result = pComment.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.getLength() == txt.length());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Comment::LineMultiplePrependedWithWhitelines")
{
	std::string txt =
		"		// This is a comment\n"
		"		// Another comment";

	gscript::ParserComment pComment;
	auto result = pComment.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.getLength() == txt.length());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Comment::LineMultipleWithSpecialCharacters")
{
	std::string txt =
		"// This is a comment, \n"
		"// Another comment";

	gscript::ParserComment pComment;
	auto result = pComment.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.getLength() == txt.length());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Comment::LineMultipleWithBlankLines")
{
	std::string txt =
		"// This is a comment\n"
		"\n"
		"// Another comment";

	gscript::ParserComment pComment;
	auto result = pComment.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.getLength() == txt.length());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("Parser::Comment::Block")
{
	{
		std::string txt = "/* This is a block comment */";

		gscript::ParserComment pComment;
		auto result = pComment.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(result.getLength() == txt.length());
		REQUIRE(result.result.end == txt.end());
	}
}

TEST_CASE("Parser::Comment::BlockMultiline")
{
	{
		std::string txt =
			"/* This is a block comment\n"
			"consisting of multiple lines.\n"
			"Something\n"
			"This is the last line*/";

		gscript::ParserComment pComment;
		auto result = pComment.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(result.getLength() == txt.length());
		REQUIRE(result.result.end == txt.end());
	}
}