#include "common.h"
#include "gscript/parser/pFieldDeclaration.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pFuncCall.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserFieldDeclarationSimple")
{
	std::string txt = "type somefield;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(pField.accessSpecifier.getModifier() == gscript::MODIFIER_T::M_NONE);
}

TEST_CASE("ParserFieldDeclarationPrivate")
{
	std::string txt = "private type somefield;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(pField.accessSpecifier.getModifier() & Bitfield(gscript::MODIFIER_T::M_ACCESS_PRIVATE));
}

TEST_CASE("ParserFieldDeclarationWithValue")
{
	std::string txt = "type somefield = 42;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pField.value.components.at(0))->value == "42");
	REQUIRE(pField.accessSpecifier.getModifier() == gscript::MODIFIER_T::M_NONE);
}

TEST_CASE("ParserFieldDeclarationWithStatement")
{
	std::string txt = "type somefield = somefunc();";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pField.value.components.at(0))->name == "somefunc");
	REQUIRE(pField.accessSpecifier.getModifier() == gscript::MODIFIER_T::M_NONE);
}

TEST_CASE("ParserFieldDeclarationFailureSingleWord")
{
	std::string txt = "somefield";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserFieldDeclarationFailureNoSemicolon")
{
	std::string txt = "type somefield";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got empty string");
}