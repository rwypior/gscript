#ifndef _h_gscript_parser_info
#define _h_gscript_parser_info

#include <string>

namespace gscript
{
	/// Contains additional information about parsing entity
	class ParserInfo
	{
	public:
		std::string file;
		size_t line;

		ParserInfo(const std::string& file = "", size_t line = 0)
			: file(file)
			, line(line)
		{
		}

		operator std::string() const
		{
			return this->file + ":" + std::to_string(this->line);
		}
	};
}

#endif