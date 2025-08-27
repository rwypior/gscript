#ifndef _h_gscript_parsestatus
#define _h_gscript_parsestatus

#include "IteratorRange.hpp"

#include <string>
#include <memory>

namespace gscript
{
	class ParserEntity;
	class ParseResult;

	struct ParseDetails
	{
		ParseDetails(const std::string& file = "", size_t line = 0, const std::string& message = "");
		ParseDetails(const StringIteratorRange& itrange, const std::string& message = "");
		ParseDetails(const ParseResult& result, const std::string& message = "");

		operator std::string() const;

		std::string file = "";
		size_t line = 0;
		std::string message = "";
	};

	class ParseResult
	{
	public:
		enum STATUS_T
		{
			S_VOID = 0,
			S_OK = 1,
			S_WARNING = 2,
			S_FATAL = 3,
			S_COMMENT = 16
		};

		STATUS_T status = STATUS_T::S_VOID;
		StringIteratorRange result;
		std::shared_ptr<ParserEntity> subResult = nullptr;
		ParseDetails details;

		ParseResult(STATUS_T status = STATUS_T::S_VOID, const ParseDetails& details = {});
		ParseResult(STATUS_T status, StringIteratorRange result, std::shared_ptr<ParserEntity> subResult = nullptr, const ParseDetails& details = {});
		ParseResult(STATUS_T status, std::string::iterator begin, std::string::iterator end, const ParseDetails& details = {});
		ParseResult(const ParseResult& b);

		ParseResult operator=(const ParseResult& b);
		std::string getWord() const;

		bool isOk() const;
		bool isComment() const;
		unsigned int getLength() const;
	};
}

#endif