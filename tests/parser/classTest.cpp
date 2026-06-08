#include "common.h"
#include "gscript/parser/pClass.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Class::Simple")
{
	std::string txt =
		"class MyClass {\n"
		"}"
		;

	gscript::ParserClass pClass;
	auto result = pClass.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pClass.name == "MyClass");
	REQUIRE(pClass.methods.empty());
}

TEST_CASE("Parser::Class::WithConstructor")
{
	std::string txt =
		"class MyClass {\n"
		"	MyClass() {\n"
		"		a_statement;"
		"	}\n"
		"}"
		;

	gscript::ParserClass pClass;
	auto result = pClass.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pClass.name == "MyClass");
	REQUIRE(pClass.methods.size() == 1);
	REQUIRE(pClass.methods.at(0).name == "MyClass");
	REQUIRE(pClass.methods.at(0).arglist.parameters.empty());
}

TEST_CASE("Parser::Class::FailureWithStatement")
{
	std::string txt =
		"class MyClass {\n"
		"a_statement;\n"
		"}"
		;

	gscript::ParserClass pClass;
	auto result = pClass.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(pClass.name == "MyClass");
	REQUIRE(result.details.line == 2);
	REQUIRE(result.details.message == "Expected one of: constructor, method, field; got \"a_statement;\"");
}

TEST_CASE("Parser::Class::WithFunction")
{
	std::string txt =
		"class MyClass {\n"
		"	int fnc() {\n"
		"		return 42;"
		"	}\n"
		"}"
		;

	gscript::ParserClass pClass;
	auto result = pClass.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pClass.name == "MyClass");
	REQUIRE(pClass.methods.size() == 1);
	REQUIRE(pClass.methods.at(0).name == "fnc");
	REQUIRE(pClass.methods.at(0).arglist.parameters.empty());
}