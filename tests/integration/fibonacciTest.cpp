#include "common.h"
#include "gscript/script.hpp"
#include "gscript/extension/ConsoleExtension.hpp"
#include "gscript/StringUtils.hpp"

#include <catch2/catch_all.hpp>

#include <ostream>

TEST_CASE("Integration::FibonacciTest::Basic")
{
	std::string txt =
		R"GSCRIPT(
#import <console>

int fib(int n) {
    if (n <= 1)
        return n;

	int a = fib(n - 1);
	int b = fib(n - 2);
    return a + b;
}

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		Console::println(tostring(fib(10)));
		
		return 0;
	}
}
		)GSCRIPT";

	std::ostringstream out;

	gscript::Script script;
	script.loadDefaultExtensions();
	static_cast<gscript::ConsoleExtension*>(script.findExtension("Console"))->out = &out;
	
	bool compileResult = script.compile(txt);
	REQUIRE(compileResult);

	int res = script.run(0, nullptr);
	
	REQUIRE(res == 0);
	REQUIRE(gscript::trim_copy(out.str()) == "55");
}