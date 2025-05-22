#ifndef _h_gscript_parsestatus
#define _h_gscript_parsestatus

#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
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

		STATUS_T status;
		IteratorRange<std::string> result;
		void *subResult;

		ParseResult(STATUS_T status = STATUS_T::S_VOID)
			:status(status)
		{}

		ParseResult(STATUS_T status, IteratorRange<std::string> result, void *subResult = NULL)
			:status(status),
			result(result),
			subResult(subResult)
		{}

		ParseResult(STATUS_T status, std::string::iterator begin, std::string::iterator end)
			:status(status),
			result(begin, end)
		{}

		ParseResult(const ParseResult &b)
			:status(b.status),
			result(b.result),
			subResult(b.subResult)
		{
		}

		ParseResult operator =(const ParseResult &b)
		{
			this->status = b.status;
			this->result = b.result;
			this->subResult = b.subResult;
			return *this;
		}

		std::string getWord() const
		{
			return std::string(result.begin, result.end);
		}

		bool isOk()
		{
			return this->status == STATUS_T::S_OK;
		}

		bool isComment()
		{
			return this->status == STATUS_T::S_COMMENT;
		}

		unsigned int getLength()
		{
			return this->result.end - this->result.begin;
		}
	};
}

#endif