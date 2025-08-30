#include "common.h"
#include "gscript/parser/pFunction.hpp"
#include "gscript/parser/pStatement.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserFunctionSimple")
{
	std::string txt =
		"void myfunc() {\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "myfunc");
	REQUIRE(pFunc.returnTypeName == "void");
}

TEST_CASE("ParserFunctionFailureNoArglist")
{
	std::string txt =
		"void myfunc {\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"(\", got \"{\"");
}

TEST_CASE("ParserFunctionFailureNoReturnType")
{
	std::string txt =
		"myfunc {\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserFunctionWithStatements")
{
	std::string txt =
		"string myfunc() {\n"
		"	statement_1;\n"
		"	statement_2;\n"
		"	statement_3;\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "myfunc");
	REQUIRE(pFunc.returnTypeName == "string");
	REQUIRE(pFunc.body.statements.size() == 3);
	REQUIRE(std::dynamic_pointer_cast<gscript::ParserStatement>(pFunc.body.statements.at(0)));
	REQUIRE(std::dynamic_pointer_cast<gscript::ParserStatement>(pFunc.body.statements.at(1)));
	REQUIRE(std::dynamic_pointer_cast<gscript::ParserStatement>(pFunc.body.statements.at(2)));
}

TEST_CASE("ParserFunctionFailureNoEnclosure")
{
	std::string txt =
		"string myfunc() {\n"
		"	statement_1;\n"
		"	statement_2;\n"
		"	statement_3;\n"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"}\", got empty string");
	// TODO - need to fix line numbers in errors - line 4 or 5 maybe
}