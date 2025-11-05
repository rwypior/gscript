#include "gscript/parser/ParseResult.hpp"

namespace gscript
{
	// Details

	ParseDetails::ParseDetails(const std::string& file, size_t line, const std::string& message, size_t newlines)
		: file(file)
		, line(line)
		, message(message)
		, newlines(newlines)
	{
	}

	ParseDetails::ParseDetails(size_t newlines = 0)
		: newlines(newlines)
	{
	}

	ParseDetails::ParseDetails(const StringIteratorRange& itrange, const std::string& message)
		: file(itrange.getFile())
		, line(itrange.getLine())
		, message(message)
	{
	}

	ParseDetails::ParseDetails(const ParseResult& result, const std::string& message)
		: ParseDetails(result.details.file, result.details.line, message.empty() ? result.details.message : message)
	{
	}

	ParseDetails ParseDetails::withMessage(const std::string& msg) const
	{
		return ParseDetails(this->file, this->line, msg, this->newlines);
	}

	ParseDetails::operator std::string() const
	{
		std::string msg = this->file + ":" + std::to_string(this->line);
		if (!message.empty())
			msg += " - " + this->message;
		return msg;
	}

	// Result

	ParseResult::ParseResult(Status status, const ParseDetails& details)
		: status(status)
		, subResult(nullptr)
		, details(details)
	{
	}

	ParseResult::ParseResult(Status status, StringIteratorRange result, std::shared_ptr<ParserEntity> subResult, const ParseDetails& details)
		: status(status)
		, result(result)
		, subResult(subResult)
		, details(details)
	{
	}

	ParseResult::ParseResult(Status status, std::string::iterator begin, std::string::iterator end, const ParseDetails& details)
		: status(status)
		, result(begin, end)
		, details(details)
	{
	}

	ParseResult::ParseResult(const ParseResult& b)
		: status(b.status)
		, result(b.result)
		, subResult(b.subResult)
		, details(b.details)
	{
	}

	ParseResult ParseResult::as(Status status) const
	{
		return ParseResult(status, this->result, this->subResult, this->details);
	}

	ParseResult ParseResult::operator=(const ParseResult& b)
	{
		this->status = b.status;
		this->result = b.result;
		this->subResult = b.subResult;
		this->details = b.details;
		return *this;
	}

	std::string ParseResult::getWord() const
	{
		return std::string(result.begin, result.end);
	}

	bool ParseResult::isOk() const
	{
		return this->status == Status::Ok;
	}

	bool ParseResult::isFatal() const
	{
		return this->status == Status::Fatal;
	}

	unsigned int ParseResult::getLength() const
	{
		return this->result.end - this->result.begin;
	}
}