#ifndef _h_gscript_extension
#define _h_gscript_extension

#include "defs.hpp"
#include "lib.hpp"

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
		void load(Script& script);
		virtual std::string getName() const;

	protected:
		SCRIPT_API virtual void run(Script &script) = 0;

	private:
		bool loaded = false;
	};
}

#endif