#ifndef _h_gscript_parseexception
#define _h_gscript_parseexception

#include <exception>
#include <string>
#include <memory>

namespace gscript
{
	class ParseResult;

	class ParseException : public std::exception
	{
	public:
		ParseException(const ParseResult& parseResult);
		ParseException(const ParseException& parseException);
		~ParseException();
		virtual const char* what() const override;

	protected:
		std::string msg;
	};
}

#endif