#include "common.h"
#include "gscript/parser/pIf.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserIfSimple")
{
	std::string txt = "if (myvar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfSingleStatement")
{
	std::string txt = "if (myvar) statement;";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfWithBody")
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

TEST_CASE("ParserIfFailureNoArgList")
{
	std::string txt = "if";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected argument list");
}

TEST_CASE("ParserIfFailureNoBody")
{
	std::string txt = "if(something)\n";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected statement");
}

TEST_CASE("ParserIfFailureMultipleStatements")
{
	std::string txt = "if(a; b; c)\n";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \")\", got \"; b; c)\"");
}

TEST_CASE("ParserIfElse")
{
	std::string txt = 
		"if(myvar) {}\n"
		"else {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfElseSingleStatements")
{
	std::string txt = 
		"if(myvar) statement;\n"
		"else another_statement;";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfElseIf")
{
	std::string txt = 
		"if(myvar) {}\n"
		"else if(othervar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfElseIfSingleStatements")
{
	std::string txt = 
		"if(myvar) statement;\n"
		"else if(othervar) another_statement;";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfElseIfElse")
{
	std::string txt = 
		"if(myvar) {}\n"
		"else if(othervar) {}\n"
		"else {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfElseIfElseSingleStatements")
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

TEST_CASE("ParserIfCommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"if (myvar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfCommentLineBeforeTwice")
{
	std::string txt = 
		"// This is a comment, \n"
		"// And another one\n"
		"if (myvar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfCommentBlockBetweenIfAndArglist")
{
	std::string txt = "if /* This is a comment */ (myvar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfCommentBlockBetweenArglistAndBlock")
{
	std::string txt = "if (myvar) /* This is a comment */ {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfCommentBlockInArglist")
{
	std::string txt = "if (myvar /* This is a comment */) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfCommentBlockInElseArglist")
{
	std::string txt = 
		"if (myvar) {}\n"
		"else if (/* This is a comment */ anothervar) {}";

	gscript::ParserIf pif;
	auto result = pif.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserIfCommentsBetweenIfElses")
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