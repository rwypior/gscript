#ifndef _h_gscript_script
#define _h_gscript_script

#include "gscript/lib.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/function.hpp"

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>

namespace gscript
{
	class ScriptExtension;

	class Script
	{
	public:
		constexpr static char entryPointClass[] = "EntryPoint";

	public:
		SCRIPT_API Script();
		SCRIPT_API Script(const std::string &path);
		SCRIPT_API Script(const std::string &path, std::shared_ptr<ScriptNamespace> mainScope);
		SCRIPT_API Script(const std::string &path, Script &parent);
		SCRIPT_API ~Script();

		SCRIPT_API void extend(ScriptExtension *ext, const std::string &name = "");
		SCRIPT_API void loadDefaultExtensions();
		SCRIPT_API const std::unordered_map<std::string, std::shared_ptr<ScriptExtension>>& getExtensions() const;
		SCRIPT_API ScriptExtension *findExtension(const std::string &name);

		SCRIPT_API bool compile();
		SCRIPT_API bool compile(const std::string &source);
		SCRIPT_API int run(int argc, char **argv);
		
		SCRIPT_API ScriptNamespace* getMainScope();

	protected:
		std::shared_ptr<ScriptNamespace> mainScope = nullptr; // Must be shared in case the script is imported by another script
		std::unordered_map<std::string, std::shared_ptr<ScriptExtension>> extensions;

		SCRIPT_API void init();
		SCRIPT_API void import(const std::string &path);

		SCRIPT_API static std::string loadSource(const std::string& path);

	private:
		std::string path;
		bool isExtern = false;
	};
}

#endif