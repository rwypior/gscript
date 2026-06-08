#include "common.h"
#include "gscript/parser/pVarDeclaration.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pFuncCall.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::VarDeclaration::Simple")
{
	std::string txt = "int myvar;";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(pVar.type == "int");
	REQUIRE(pVar.value.components.empty());
}

TEST_CASE("Parser::VarDeclaration::WithValue")
{
	std::string txt = "int myvar = 42;";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(pVar.type == "int");
	REQUIRE(pVar.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pVar.value.components.at(0))->value == "42");
}

TEST_CASE("Parser::VarDeclaration::WithValueStatement")
{
	std::string txt = "int myvar = somefunc();";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(pVar.type == "int");
	REQUIRE(pVar.value.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pVar.value.components.at(0))->name == "somefunc");
}

TEST_CASE("Parser::VarDeclaration::FailureNoSemicolon")
{
	std::string txt = "int myvar";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got empty string");
}

TEST_CASE("Parser::VarDeclaration::FailureNoAssignOperator")
{
	std::string txt = "int myvar 42";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got \"4\"");
}

TEST_CASE("Parser::VarDeclaration::FailureNoType")
{
	std::string txt = "myvar";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::VarDeclaration::CommentLineBeforeType")
{
	std::string txt = 
		"// This is a comment\n"
		"int myvar;";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(pVar.type == "int");
	REQUIRE(pVar.value.components.empty());
}

TEST_CASE("Parser::VarDeclaration::CommentBlockBetweenTypeAndName")
{
	std::string txt = 
		"int /* This is a comment */ myvar;";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(pVar.type == "int");
	REQUIRE(pVar.value.components.empty());
}

TEST_CASE("Parser::VarDeclaration::CommentLineAfterEnd")
{
	// This test is probably redundant

	std::string txt = 
		"int myvar; // This is a comment";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(pVar.type == "int");
	REQUIRE(pVar.value.components.empty());
}