#include "common.h"
#include "gscript/parser/pFunction.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pReturn.hpp"
#include "gscript/parser/pLiteral.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Function::Simple")
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

TEST_CASE("Parser::Function::WithParams")
{
	std::string txt =
		"void myfunc(string str) {\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "myfunc");
	REQUIRE(pFunc.returnTypeName == "void");
	REQUIRE(pFunc.arglist.parameters.at(0).type == "string");
	REQUIRE(pFunc.arglist.parameters.at(0).name == "str");
}

TEST_CASE("Parser::Function::FailureNoArglist")
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

TEST_CASE("Parser::Function::FailureNoReturnType")
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

TEST_CASE("Parser::Function::WithStatements")
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

TEST_CASE("Parser::Function::FailureNoEnclosure")
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

TEST_CASE("Parser::Function::Return")
{
	std::string txt =
		"int myfunc() {\n"
		"	return 42;\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "myfunc");
	REQUIRE(pFunc.returnTypeName == "int");

	auto ret = std::static_pointer_cast<gscript::ParserReturn>(pFunc.body.statements.at(0));
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(ret->value.components.at(0))->value == "42");
}

TEST_CASE("Parser::Function::CommentLineBefore")
{
	std::string txt =
		"// This is a comment\n"
		"void myfunc() {\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "myfunc");
	REQUIRE(pFunc.returnTypeName == "void");
}

TEST_CASE("Parser::Function::CommentLineBeforeBlockEnd")
{
	std::string txt =
		"void myfunc() {\n"
		"// This is a comment\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "myfunc");
	REQUIRE(pFunc.returnTypeName == "void");
}

TEST_CASE("Parser::Function::CommentBlockAfterType")
{
	std::string txt =
		"void /* This is a comment */ myfunc() {\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "myfunc");
	REQUIRE(pFunc.returnTypeName == "void");
}

TEST_CASE("Parser::Function::CommentBlockAfterName")
{
	std::string txt =
		"void myfunc /* This is a comment */ () {\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "myfunc");
	REQUIRE(pFunc.returnTypeName == "void");
}

TEST_CASE("Parser::Function::CommentBlockAfterArglist")
{
	std::string txt =
		"void myfunc() /* This is a comment */ {\n"
		"}"
		;

	gscript::ParserFunction pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "myfunc");
	REQUIRE(pFunc.returnTypeName == "void");
}

TEST_CASE("Parser::Function::CommentBlocksBetweenStatements")
{
	std::string txt =
		"string myfunc() {\n"
		"	/* Comment 1 */\n"
		"	statement_1;\n"
		"	/* Comment 2 */\n"
		"	statement_2;\n"
		"	/* Comment 3 */\n"
		"	statement_3;\n"
		"	/* Comment 4 */\n"
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