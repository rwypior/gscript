#include "parseexception.hpp"
#include "parser/ParseResult.hpp"

namespace
{
	std::string getParseExceptionMsg(const gscript::ParseResult& parseResult)
	{
		return std::string("Parsing error - ") + parseResult.details.operator std::string();
	}
}

namespace gscript
{
	ParseException::ParseException(const ParseResult& parseResult)
		: std::exception()
		, msg(getParseExceptionMsg(parseResult))
	{
	}

	ParseException::ParseException(const ParseException& parseException)
		: std::exception()
		, msg(parseException.msg)
	{
	}

	ParseException::~ParseException() = default;

	const char* ParseException::what() const
	{
		return this->msg.c_str();
	}
}