#ifndef _h_gscript_test_common
#define _h_gscript_test_common

#include "gscript/script.hpp"
#include "gscript/scope.hpp"
#include "gscript/globalNamespace.hpp"

class GscriptTest
{
public:
	GscriptTest()
		: globalNamespace(script)
	{
	}

protected:
	gscript::Script script;
	gscript::ScriptGlobalNamespace globalNamespace;
};

#endif