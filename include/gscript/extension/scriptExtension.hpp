#ifndef _h_gscript_extension
#define _h_gscript_extension

#include "gscript/defs.hpp"
#include "gscript/lib.hpp"

#include <string>

namespace gscript
{
	class Script;

	class ScriptExtension
	{
	public:
		class ExtensionFunction
		{
		public:
			virtual void setup() {};
		};

	public:
		SCRIPT_API void load(Script& script);
		SCRIPT_API virtual std::string getName() const;

	protected:
		SCRIPT_API virtual void run(Script &script) = 0;

	private:
		bool loaded = false;
	};
}

#endif