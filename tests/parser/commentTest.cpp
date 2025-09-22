#include "common.h"
#include "gscript/parser/pComment.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserCommentSingleLine")
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

TEST_CASE("ParserCommentLineMultiple")
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

TEST_CASE("ParserCommentBlock")
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

TEST_CASE("ParserCommentBlockMultiline")
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