#include "common.h"
#include "gscript/parser/pFor.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::For::Simple")
{
	std::string txt = "for (int i; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::WithBody")
{
	std::string txt = 
		"for (int i; i < 5; i++)\n"
		"{\n"
		"somevar += i;\n"
		"}\n";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::SingleStatementBody")
{
	std::string txt = "for (int i; i < 10; i++) some_statement();";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pfor.body.body.statements.size() == 1);
}

TEST_CASE("Parser::For::FailureNoArgList")
{
	std::string txt = "for";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::For::FailureNoBody")
{
	std::string txt = "for(int i; i < 10; i++)\n";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::For::NoFirstParameter")
{
	std::string txt = "for (; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::NoSecondParameter")
{
	std::string txt = "for (int i; ; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::NoThirdParameter")
{
	std::string txt = "for (int i; i < 10; ) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::OnlyFirstParameter")
{
	std::string txt = "for (int i; ; ) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::OnlySecondParameter")
{
	std::string txt = "for (; i < 10; ) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::OnlyThirdParameter")
{
	std::string txt = "for (; ; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::CommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"for (int i; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::CommentBlockBetweenForAndArglist")
{
	std::string txt = "for /* This is a comment */ (int i; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::CommentBlockAfterVar")
{
	std::string txt = "for (int i; /* This is a comment */ i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::CommentBlockAfterCondition")
{
	std::string txt = "for (int i; i < 10; /* This is a comment */ i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::CommentBlockAfterStep")
{
	std::string txt = "for (int i; i < 10; i++ /* This is a comment */) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::CommentBlockBeforeVarSemicolon")
{
	std::string txt = "for (int i /* This is a comment */; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::For::CommentBlockBeforeBody")
{
	std::string txt = "for (int i; i < 10; i++) /* This is a comment */ {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}