#include "common.h"
#include "gscript/parser/pBlock.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Block::Simple")
{
	std::string txt = 
		"{\n"
		"a_statement;\n"
		"}"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Block::OneLiner")
{
	{
		std::string txt =
			"a_statement;";

		gscript::ParserBlock pBlock;
		auto result = pBlock.parse(txt);

		REQUIRE(result.isOk());
	}

	{
		std::string txt =
			"			    a_statement;		";

		gscript::ParserBlock pBlock;
		auto result = pBlock.parse(txt);

		REQUIRE(result.isOk());
	}
}

TEST_CASE("Parser::Block::OneLinerNoSemicolon")
{
	std::string txt = 
		"		a_statement"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got empty string");
}

TEST_CASE("Parser::Block::FailureNoSemicolon")
{
	std::string txt = 
		"{\n"
		"a_statement\n"
		"}"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got \"}\"");
}

TEST_CASE("Parser::Block::FailureNoEnclosure")
{
	std::string txt = 
		"{\n"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"}\", got empty string");
}

TEST_CASE("Parser::Block::FailureEmptyString")
{
	std::string txt = "";

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected statement");
}

TEST_CASE("Parser::Block::CommentBeforeBlock")
{
	std::string txt =
		"// This is a comment\n"
		"{\n"
		"a_statement;\n"
		"}"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Block::CommentBlockBeforeBlock")
{
	std::string txt =
		"/* This is a block comment */\n"
		"{\n"
		"a_statement;\n"
		"}"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Block::CommentBlockAfterBlock")
{
	std::string txt =
		"{\n"
		"/* This is a block comment */\n"
		"a_statement;\n"
		"}"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::Block::CommentBlockBeforeBlockEnd")
{
	std::string txt =
		"{\n"
		"a_statement;\n"
		"/* This is a block comment */\n"
		"}"
		;

	gscript::ParserBlock pBlock;
	auto result = pBlock.parse(txt);

	REQUIRE(result.isOk());
}