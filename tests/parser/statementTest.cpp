#include "common.h"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pVar.hpp"
#include "gscript/parser/pFuncCall.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pOperator.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Statement::SingleVariable")
{
	std::string txt = "some_variable;";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(0))->name == "some_variable");
}

TEST_CASE("Parser::Statement::SubStatement")
{
	std::string txt = "some_variable";

	gscript::ParserStatement pStmt(true);
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(0))->name == "some_variable");
}

TEST_CASE("Parser::Statement::SingleFunctionCall")
{
	std::string txt = "some_func();";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pStmt.components.at(0))->name == "some_func");
}

TEST_CASE("Parser::Statement::SingleLiteral")
{
	std::string txt = "\"somestring\";";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(0))->value == "somestring");
}

TEST_CASE("Parser::Statement::Addition")
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

TEST_CASE("Parser::Statement::AdditionNoSpaces")
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

TEST_CASE("Parser::Statement::MemberAccessor")
{
	std::string txt = "myobject.anotherobject.myvar;";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 5);
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(0))->name == "myobject");
	REQUIRE(std::dynamic_pointer_cast<gscript::ParserOperatorMemberAccessor>(pStmt.components.at(1)));
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(2))->name == "anotherobject");
	REQUIRE(std::dynamic_pointer_cast<gscript::ParserOperatorMemberAccessor>(pStmt.components.at(3)));
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(4))->name == "myvar");
}

TEST_CASE("Parser::Statement::CommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"some_variable;";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(0))->name == "some_variable");
}

TEST_CASE("Parser::Statement::CommentBlockBeforeSemicolon")
{
	std::string txt = "some_variable /* This is a comment */ ;";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(0))->name == "some_variable");
}

TEST_CASE("Parser::Statement::CommentBlockAfterSemicolon")
{
	std::string txt = "some_variable; /* This is a comment */";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(0))->name == "some_variable");
}

TEST_CASE("Parser::Statement::SubstatementCommentBlockAtEnd")
{
	std::string txt = "some_variable /* This is a comment */";

	gscript::ParserStatement pStmt(true);
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 1);
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pStmt.components.at(0))->name == "some_variable");
}

TEST_CASE("Parser::Statement::CommentBlocksBeforeComponents")
{
	std::string txt = "1 /* Comment 1 */ + /* Comment 2 */ 2;";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 3);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(0))->value == "1");
	REQUIRE(std::static_pointer_cast<gscript::ParserOperatorAdd>(pStmt.components.at(1))->getChar() == "+");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(2))->value == "2");
}

TEST_CASE("Parser::Statement::CommentLinesBeforeComponents")
{
	std::string txt = 
		"1\n"
		"// This is a comment\n"
		"+\n"
		"// Another comment\n"
		"2;";

	gscript::ParserStatement pStmt;
	auto result = pStmt.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pStmt.components.size() == 3);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(0))->value == "1");
	REQUIRE(std::static_pointer_cast<gscript::ParserOperatorAdd>(pStmt.components.at(1))->getChar() == "+");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pStmt.components.at(2))->value == "2");
}