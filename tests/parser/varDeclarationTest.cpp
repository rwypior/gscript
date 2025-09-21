#include "common.h"
#include "gscript/parser/pVarDeclaration.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pFuncCall.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserVarDeclarationSimple")
{
	std::string txt = "int myvar;";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(pVar.type == "int");
	REQUIRE(pVar.value.components.empty());
}

TEST_CASE("ParserVarDeclarationWithValue")
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

TEST_CASE("ParserVarDeclarationWithValueStatement")
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

TEST_CASE("ParserVarDeclarationFailureNoSemicolon")
{
	std::string txt = "int myvar";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got empty string");
}

TEST_CASE("ParserVarDeclarationFailureNoAssignOperator")
{
	std::string txt = "int myvar 42";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got \"4\"");
}

TEST_CASE("ParserVarDeclarationFailureNoType")
{
	std::string txt = "myvar";

	gscript::ParserVarDeclaration pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserVarDeclarationCommentLineBeforeType")
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

TEST_CASE("ParserVarDeclarationCommentBlockBetweenTypeAndName")
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

TEST_CASE("ParserVarDeclarationCommentLineAfterEnd")
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