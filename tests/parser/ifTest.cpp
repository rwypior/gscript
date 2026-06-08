#include "common.h"
#include "gscript/parser/pIf.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::If::Simple")
{
	std::string txt = "if (myvar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::SingleStatement")
{
	std::string txt = "if (myvar) statement;";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::WithBody")
{
	std::string txt =
		"if (true)\n"
		"{\n"
		"somevar += i;\n"
		"}\n";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::FailureNoArgList")
{
	std::string txt = "if";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected argument list");
}

TEST_CASE("Parser::If::FailureNoBody")
{
	std::string txt = "if(something)\n";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected statement");
}

TEST_CASE("Parser::If::FailureMultipleStatements")
{
	std::string txt = "if(a; b; c)\n";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \")\", got \"; b; c)\"");
}

TEST_CASE("Parser::If::Else")
{
	std::string txt = 
		"if(myvar) {}\n"
		"else {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::ElseSingleStatements")
{
	std::string txt = 
		"if(myvar) statement;\n"
		"else another_statement;";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::ElseIf")
{
	std::string txt = 
		"if(myvar) {}\n"
		"else if(othervar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::ElseIfSingleStatements")
{
	std::string txt = 
		"if(myvar) statement;\n"
		"else if(othervar) another_statement;";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::ElseIfElse")
{
	std::string txt = 
		"if(myvar) {}\n"
		"else if(othervar) {}\n"
		"else {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::ElseIfElseSingleStatements")
{
	std::string txt = 
		"if(myvar) statement;\n"
		"else if(othervar) another_statement;\n"
		"else yet_another_statement;";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pif.pelse.body.body.statements.empty());
}

TEST_CASE("Parser::If::CommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"if (myvar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::CommentLineBeforeTwice")
{
	std::string txt = 
		"// This is a comment, \n"
		"// And another one\n"
		"if (myvar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::CommentBlockBetweenIfAndArglist")
{
	std::string txt = "if /* This is a comment */ (myvar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::CommentBlockBetweenArglistAndBlock")
{
	std::string txt = "if (myvar) /* This is a comment */ {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::CommentBlockInArglist")
{
	std::string txt = "if (myvar /* This is a comment */) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::CommentBlockInElseArglist")
{
	std::string txt = 
		"if (myvar) {}\n"
		"else if (/* This is a comment */ anothervar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("Parser::If::CommentsBetweenIfElses")
{
	std::string txt = 
		"if (myvar) {}\n"
		"// This is a comment\n"
		"else if (anothervar) {}\n"
		"/* This is also a comment*/\n"
		"else {}"
		;

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}