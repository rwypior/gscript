#include "common.h"
#include "gscript/parser/pVar.hpp"
#include "gscript/parser/pLiteral.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Var::Simple")
{
	std::string txt = "myvar";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
}

TEST_CASE("Parser::Var::Array")
{
	std::string txt = "myvar[42]";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pVar.arrayAccessor->statement.components.at(0))->value == "42");
}

TEST_CASE("Parser::Var::FailureEmptyString")
{
	std::string txt = "";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::Var::FailureArrayNoEnclosure")
{
	std::string txt = "myvar[";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Empty statement"); // TODO - this needs better error message
}

TEST_CASE("Parser::Var::FailureArrayEmpty")
{
	std::string txt = "myvar[]";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected non-empty statement");
}

TEST_CASE("Parser::Var::MemberAccess")
{
	// Variable found here is myobject - the rest of the path would be parsed by a statement

	std::string txt = "myobject.anotherobject.myvar";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myobject");
}

TEST_CASE("Parser::Var::CommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"myvar";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
}

TEST_CASE("Parser::Var::CommentBlockBeforeArray")
{
	std::string txt = "myvar /* This is a comment */ [42]";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pVar.arrayAccessor->statement.components.at(0))->value == "42");
}