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