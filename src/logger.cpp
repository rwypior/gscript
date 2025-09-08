#include "logger.hpp"

namespace gscript
{
	void Logger::log(const std::string& entry)
	{
		this->entries.push_back(entry);
	}

	Logger& Logger::get()
	{
		static Logger logger;
		return logger;
	}
}