#ifndef _h_gscript_scoped_call
#define _h_gscript_scoped_call

#include "gscript/runtime/entityLink.hpp"

namespace gscript
{
	class ScriptClassInstance;

	class ScriptScopedCall
	{
	public:
		virtual void setScope(ScriptClassInstance *instance) = 0;
	};
}

#endif