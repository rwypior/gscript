#include "common.h"
#include "gscript/parser/pNamespace.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Namespace::Simple")
{
	std::string txt =
		"namespace MyNamespace {\n"
		"}"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNamespace.namespaces.empty());
	REQUIRE(pNamespace.classes.empty());
	REQUIRE(pNamespace.functions.empty());
}

TEST_CASE("Parser::Namespace::FailureNoNamespaceKeyword")
{
	std::string txt =
		"MyNamespace {\n"
		"}"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"namespace\"");
}

TEST_CASE("Parser::Namespace::FailureNoOpening")
{
	std::string txt =
		"namespace MyNamespace\n"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"{\", got empty string");
}

TEST_CASE("Parser::Namespace::FailureNoEnclosure")
{
	std::string txt =
		"namespace MyNamespace {\n"
		""
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"}\", got empty string");
}

TEST_CASE("Parser::Namespace::WithClasses")
{
	std::string txt =
		"namespace MyNamespace {\n"
		"	class MyClass {\n"
		"	}\n"
		"	class MyClass2 {\n"
		"	}\n"
		"}"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNamespace.namespaces.empty());
	REQUIRE(pNamespace.classes.size() == 2);
	REQUIRE(pNamespace.functions.empty());
}

TEST_CASE("Parser::Namespace::WithFunctions")
{
	std::string txt =
		"namespace MyNamespace {\n"
		"	void fnc1(int something) {\n"
		"	}\n"
		"	bool fnc2(string blabla) {\n"
		"	}\n"
		"}"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNamespace.namespaces.empty());
	REQUIRE(pNamespace.classes.empty());
	REQUIRE(pNamespace.functions.size() == 2);
}

TEST_CASE("Parser::Namespace::WithNamespaces")
{
	std::string txt =
		"namespace MyNamespace {\n"
		"	namespace ns1 {\n"
		"	}\n"
		"	namespace ns2 {\n"
		"	}\n"
		"}"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNamespace.namespaces.size() == 2);
	REQUIRE(pNamespace.classes.empty());
	REQUIRE(pNamespace.functions.empty());
}

TEST_CASE("Parser::Namespace::WithEverything")
{
	std::string txt =
		"namespace MyNamespace {\n"
		"	namespace ns1 {\n"
		"	}\n"
		"	void fnc1(int something) {\n"
		"	}\n"
		"	class MyClass {\n"
		"	}\n"
		"	class MyClass2 {\n"
		"	}\n"
		"	namespace ns2 {\n"
		"	}\n"
		"	bool fnc2(string blabla) {\n"
		"	}\n"
		"}"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNamespace.namespaces.size() == 2);
	REQUIRE(pNamespace.classes.size() == 2);
	REQUIRE(pNamespace.functions.size() == 2);
}

TEST_CASE("Parser::Namespace::CommentBeforeNamespace")
{
	std::string txt =
		"// This is a namespace\n"
		"namespace MyNamespace {\n"
		"}"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNamespace.name == "MyNamespace");
	REQUIRE(pNamespace.namespaces.empty());
	REQUIRE(pNamespace.classes.empty());
	REQUIRE(pNamespace.functions.empty());
}

TEST_CASE("Parser::Namespace::CommentBlockBeforeNamespace")
{
	std::string txt =
		"/* This is a namespace */\n"
		"namespace MyNamespace {\n"
		"}"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNamespace.name == "MyNamespace");
	REQUIRE(pNamespace.namespaces.empty());
	REQUIRE(pNamespace.classes.empty());
	REQUIRE(pNamespace.functions.empty());
}

TEST_CASE("Parser::Namespace::CommentBeforeName")
{
	std::string txt =
		"namespace //MyNamespace {\n"
		"MyNamespace {\n"
		"}"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pNamespace.name == "MyNamespace");
	REQUIRE(pNamespace.namespaces.empty());
	REQUIRE(pNamespace.classes.empty());
	REQUIRE(pNamespace.functions.empty());
}