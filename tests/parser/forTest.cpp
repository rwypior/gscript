#include "common.h"
#include "gscript/parser/pFor.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserForSimple")
{
	std::string txt = "for (int i; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForWithBody")
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

TEST_CASE("ParserForSingleStatementBody")
{
	std::string txt = "for (int i; i < 10; i++) some_statement();";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pfor.body.body.statements.size() == 1);
}

TEST_CASE("ParserForFailureNoArgList")
{
	std::string txt = "for";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserForFailureNoBody")
{
	std::string txt = "for(int i; i < 10; i++)\n";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserForNoFirstParameter")
{
	std::string txt = "for (; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForNoSecondParameter")
{
	std::string txt = "for (int i; ; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForNoThirdParameter")
{
	std::string txt = "for (int i; i < 10; ) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForOnlyFirstParameter")
{
	std::string txt = "for (int i; ; ) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForOnlySecondParameter")
{
	std::string txt = "for (; i < 10; ) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForOnlyThirdParameter")
{
	std::string txt = "for (; ; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForCommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"for (int i; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForCommentBlockBetweenForAndArglist")
{
	std::string txt = "for /* This is a comment */ (int i; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForCommentBlockAfterVar")
{
	std::string txt = "for (int i; /* This is a comment */ i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForCommentBlockAfterCondition")
{
	std::string txt = "for (int i; i < 10; /* This is a comment */ i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForCommentBlockAfterStep")
{
	std::string txt = "for (int i; i < 10; i++ /* This is a comment */) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForCommentBlockBeforeVarSemicolon")
{
	std::string txt = "for (int i /* This is a comment */; i < 10; i++) {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserForCommentBlockBeforeBody")
{
	std::string txt = "for (int i; i < 10; i++) /* This is a comment */ {}";

	gscript::ParserFor pfor;
	auto result = pfor.parse(txt);

	REQUIRE(result.isOk());
}