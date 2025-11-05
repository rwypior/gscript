#ifndef _h_gscript_parseexception
#define _h_gscript_parseexception

#include "gscript/lib.hpp"

#include <exception>
#include <string>
#include <memory>

namespace gscript
{
	class ParseResult;

	class ParseException : public std::exception
	{
	public:
		SCRIPT_API ParseException(const ParseResult& parseResult);
		SCRIPT_API ParseException(const ParseException& parseException);
		SCRIPT_API ~ParseException();
		SCRIPT_API virtual const char* what() const override;

	protected:
		std::string msg;
	};
}

#endif