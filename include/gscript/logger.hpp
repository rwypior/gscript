#ifndef _h_gscript_logger
#define _h_gscript_logger

#include <string>
#include <vector>
#include <sstream>

#ifdef DEBUG
#	define gs_log(what) do { gscript::Logger::get().log((std::stringstream() << what).str()); } while(0)
#	define gs_debug_log() auto& GS_DEBUG_LOGGER = gscript::Logger::get();
#else
#	define gs_log(what)
#	define gs_debug_log()
#endif

#ifdef DEBUG
namespace gscript
{
	class Logger
	{
	public:
		void log(const std::string& entry);

		static Logger& get();

	private:
		Logger() = default;

		std::vector<std::string> entries;
	};
}
#endif

#endif