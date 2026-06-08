#include "common.h"
#include "gscript/parser/pFieldDeclaration.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pFuncCall.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::FieldDeclaration::Simple")
{
	std::string txt = "type somefield;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(pField.accessSpecifier.getModifier() == gscript::Modifier::None);
}

TEST_CASE("Parser::FieldDeclaration::Private")
{
	std::string txt = "private type somefield;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(pField.accessSpecifier.getModifier() & Bitfield(gscript::Modifier::AccessPrivate));
}

TEST_CASE("Parser::FieldDeclaration::WithValue")
{
	std::string txt = "type somefield = 42;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pField.value.components.at(0))->value == "42");
	REQUIRE(pField.accessSpecifier.getModifier() == gscript::Modifier::None);
}

TEST_CASE("Parser::FieldDeclaration::WithStatement")
{
	std::string txt = "type somefield = somefunc();";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pField.value.components.at(0))->name == "somefunc");
	REQUIRE(pField.accessSpecifier.getModifier() == gscript::Modifier::None);
}

TEST_CASE("Parser::FieldDeclaration::FailureSingleWord")
{
	std::string txt = "somefield";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::FieldDeclaration::FailureNoSemicolon")
{
	std::string txt = "type somefield";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \";\", got empty string");
}

TEST_CASE("Parser::FieldDeclaration::CommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"type somefield;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(pField.accessSpecifier.getModifier() == gscript::Modifier::None);
}

TEST_CASE("Parser::FieldDeclaration::CommentBlockBetweenTypeAndName")
{
	std::string txt = "type /* This is a comment */ somefield;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(pField.accessSpecifier.getModifier() == gscript::Modifier::None);
}

TEST_CASE("Parser::FieldDeclaration::CommentBlockBetweenPrivateAndType")
{
	std::string txt = "private /* This is a comment */ type somefield;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(pField.accessSpecifier.getModifier() & Bitfield(gscript::Modifier::AccessPrivate));
}

TEST_CASE("Parser::FieldDeclaration::CommentLineBeforePrivate")
{
	std::string txt = 
		"// This is a comment\n"
		"private type somefield;";

	gscript::ParserFieldDeclaration pField;
	auto result = pField.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pField.name == "somefield");
	REQUIRE(pField.type == "type");
	REQUIRE(pField.accessSpecifier.getModifier() & Bitfield(gscript::Modifier::AccessPrivate));
}