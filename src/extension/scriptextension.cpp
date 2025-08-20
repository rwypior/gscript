#include "extension/scriptExtension.hpp"

namespace gscript
{
	void ScriptExtension::load(Script& script)
	{
		if (!this->loaded)
		{
			this->loaded = true;
			this->run(script);
		}
	}

	std::string ScriptExtension::getName() const
	{
		return "";
	}
}