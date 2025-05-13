#ifndef _h_script_extension
#define _h_script_extension

#include "defs.hpp"

#include "lib.hpp"

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

	private:
		bool loaded = false;

	protected:
		SCRIPT_API virtual void run(Script &script) = 0;

	public:
		void load(Script &script)
		{
			if (!this->loaded)
			{
				this->loaded = true;
				this->run(script);
			}
		}

		virtual std::string getName() const { return ""; }
	};
}

#endif