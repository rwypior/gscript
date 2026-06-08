#include "common.h"
#include "gscript/parser/pFunction.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Integration::CommentsFunction::Basic")
{
	{
		std::string txt =
			"int fib(int n) {\n"
			"\n"
			"	// If given value is lesser or equal to one, \n"
			"	// return the value as it is\n"
			"	if (n <= 1)\n"
			"		return n;\n"
			"\n"
			"	// Calculate the fibonacci recursively \n"
			"	return fib(n - 1) + fib(n - 2);\n"
			"}\n"
			"\n"
			"class Main : EntryPoint\n"
			"{\n"
			"	int run(int argc, string[] argv)\n"
			"	{\n"
			"		Console::println(\"asd\");\n"
			"		myFunction();\n"
			"\n"
			"		SomeClass sc = new SomeClass();\n"
			"		sc.method();\n"
			"\n"
			"		return 0;\n"
			"	}\n"
			"}\n";

		gscript::ParserFunction pFunc;
		auto result = pFunc.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pFunc.name == "fib");
	}
}