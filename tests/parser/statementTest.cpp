#include "common.h"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pVar.hpp"
#include "gscript/parser/pFuncCall.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pOperator.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserStatementSingleVariable")
{
	std::string txt = "some_variable;";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(0))->name == "some_variable");
}

TEST_CASE("ParserStatementSingleFunctionCall")
{
	std::string txt = "some_func();";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pStmt.components.at(0))->name == "some_func");
}

TEST_CASE("ParserStatementSingleLiteral")
{
	std::string txt = "\"somestring\";";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(0))->value == "somestring");
}

TEST_CASE("ParserStatementAddition")
{
	std::string txt = "1 + 2;";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 3);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(0))->value == "1");
	REQUIRE(std::static_pointer_cast<gscript::ParserOperatorAdd>(pStmt.components.at(1))->getChar() == "+");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(2))->value == "2");
}

TEST_CASE("ParserStatementAdditionNoSpaces")
{
	std::string txt = "1+2;";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 3);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(0))->value == "1");
	REQUIRE(std::static_pointer_cast<gscript::ParserOperatorAdd>(pStmt.components.at(1))->getChar() == "+");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(2))->value == "2");
}