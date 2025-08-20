#ifndef _h_gscript_script
#define _h_gscript_script

#include "runtime/scriptValue.hpp"
#include "runtime/globalNamespace.hpp"
#include "runtime/function.hpp"
#include "lib.hpp"

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>

namespace gscript
{
	class ScriptExtension;
	class ScriptGlobalNamespace;

	class Script
	{
	public:
		SCRIPT_API Script();
		SCRIPT_API Script(const std::string &path);
		SCRIPT_API Script(const std::string &path, std::shared_ptr<ScriptGlobalNamespace> mainScope);
		SCRIPT_API Script(const std::string &path, Script &parent);
		~Script() = default;

		SCRIPT_API void extend(ScriptExtension *ext, const std::string &name = "");
		SCRIPT_API void loadDefaultExtensions();
		SCRIPT_API ScriptExtension *findExtension(const std::string &name);

		SCRIPT_API bool compile();
		SCRIPT_API int run(int argc, char **argv);
		
		SCRIPT_API ScriptGlobalNamespace *getMainScope();

	protected:
		std::shared_ptr<ScriptGlobalNamespace> mainScope = nullptr;
		std::unordered_map<std::string, std::shared_ptr<ScriptExtension>> extensions;

		void init();
		void import(const std::string &path);

		static std::string loadSource(const std::string& path);

	private:
		std::string path;
		bool isExtern = false;
	};
}

#endif