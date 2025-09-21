#include "common.h"
#include "gscript/parser/pNamespace.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserNamespaceSimple")
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

TEST_CASE("ParserNamespaceFailureNoNamespaceKeyword")
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

TEST_CASE("ParserNamespaceFailureNoOpening")
{
	std::string txt =
		"namespace MyNamespace\n"
		;

	gscript::ParserNamespace pNamespace;
	auto result = pNamespace.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"{\", got empty string");
}

TEST_CASE("ParserNamespaceFailureNoEnclosure")
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

TEST_CASE("ParserNamespaceWithClasses")
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

TEST_CASE("ParserNamespaceWithFunctions")
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

TEST_CASE("ParserNamespaceWithNamespaces")
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

TEST_CASE("ParserNamespaceWithEverything")
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

TEST_CASE("ParserNamespaceCommentBeforeNamespace")
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

TEST_CASE("ParserNamespaceCommentBlockBeforeNamespace")
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

TEST_CASE("ParserNamespaceCommentBeforeName")
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