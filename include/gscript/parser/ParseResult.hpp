#ifndef _h_gscript_parsestatus
#define _h_gscript_parsestatus

#include "gscript/IteratorRange.hpp"

#include <string>
#include <memory>

namespace gscript
{
	class ParserEntity;
	class ParseResult;

	struct ParseDetails
	{
		ParseDetails(const std::string& file = "", size_t line = 0, const std::string& message = "", size_t newlines = 0);
		ParseDetails(size_t newlines);
		ParseDetails(const StringIteratorRange& itrange, const std::string& message = "");
		ParseDetails(const ParseResult& result, const std::string& message = "");

		ParseDetails withMessage(const std::string& msg) const;

		operator std::string() const;

		std::string file = "";
		size_t line = 0;
		std::string message = "";
		size_t newlines = 0;
	};

	class ParseResult
	{
	public:
		enum Status
		{
			Ok, // Parsing successed
			Invalid, // Invalid syntax - not fatal - continue parsing
			Fatal // Fatal parsing error - abort parsing
		};

		Status status = Status::Ok;
		StringIteratorRange result;
		std::shared_ptr<ParserEntity> subResult = nullptr;
		ParseDetails details;

		ParseResult(Status status = Status::Ok, const ParseDetails& details = {});
		ParseResult(Status status, StringIteratorRange result, std::shared_ptr<ParserEntity> subResult = nullptr, const ParseDetails& details = {});
		ParseResult(Status status, std::string::iterator begin, std::string::iterator end, const ParseDetails& details = {});
		ParseResult(const ParseResult& b);

		ParseResult as(Status status) const;

		ParseResult operator=(const ParseResult& b);
		std::string getWord() const;

		bool isOk() const;
		bool isFatal() const;
		unsigned int getLength() const;
	};
}

#endif