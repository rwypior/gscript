#ifndef _h_gscript_script
#define _h_gscript_script

#include "function.hpp"
#include "lib.hpp"

#include <string>
#include <vector>
#include <list>
#include <unordered_map>


namespace gscript
{
	class ScriptExtension;
	class ScriptGlobalNamespace;

	class Script
	{
	public:
		typedef std::unordered_map<std::string, ScriptExtension*> EXTENSIONS_CONTAINER_T;

	public:
		SCRIPT_API static ScriptNullValue *SCR_NULL;
		SCRIPT_API static ScriptBoolValue *SCR_TRUE;
		SCRIPT_API static ScriptBoolValue *SCR_FALSE;
		
		SCRIPT_API Script();
		SCRIPT_API Script(const std::string &path, const std::string &content);
		SCRIPT_API Script(const std::string &path, const std::string &content, ScriptGlobalNamespace *mainScope);
		SCRIPT_API Script(const std::string &path, const std::string &content, Script &parent);
		SCRIPT_API ~Script();

		SCRIPT_API Script &operator=(const Script &b);

		SCRIPT_API void extend(ScriptExtension *ext, const std::string &name = "");
		SCRIPT_API void loadDefaultExtensions();
		SCRIPT_API ScriptExtension *findExtension(const std::string &name);

		SCRIPT_API bool compile();
		SCRIPT_API int run(int argc, char **argv);
		
		SCRIPT_API const std::string &getContent();
		SCRIPT_API static Script load(const std::string &path, ScriptGlobalNamespace *parentMainScope = NULL);

		SCRIPT_API ScriptGlobalNamespace *getMainScope();

	protected:
		std::string content;
		ScriptGlobalNamespace *mainScope = nullptr;
		EXTENSIONS_CONTAINER_T extensions;

		void init();
		void import(const std::string &path);

	private:
		std::string path;
		bool isExtern = false;
	};
}

#endif