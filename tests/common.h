#ifndef _h_gscript_test_common
#define _h_gscript_test_common

#include "gscript/script.hpp"
#include "gscript/runtime/scope.hpp"

class GscriptTest
{
public:
	GscriptTest()
		: globalNamespace(*script.getMainScope())
	{
	}

protected:
	gscript::Script script;
	gscript::ScriptNamespace& globalNamespace;
};

#endif