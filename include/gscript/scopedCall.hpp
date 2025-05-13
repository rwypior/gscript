#ifndef _h_script_scoped_call
#define _h_script_scoped_call

#include "entityLink.hpp"

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