#include "parser/pNameSpecifier.hpp"
#include "parser/pComment.hpp"
#include "StringUtils.hpp"

namespace gscript
{
	ParserNameSpecifier::ParserNameSpecifier(const std::string &name)
		:name(name)
	{ }

	ParseResult ParserNameSpecifier::parse(StringIteratorRange itrange)
	{
		auto begin = itrange.begin;

		unsigned int commentLength = 0;
		COMMENT(itrange, itrange.begin, commentLength);

		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::Status::Invalid, COMMENT_RESULT(itrange, commentLength), nullptr, { itrange, "Expected name" });

		StringIteratorRange itrangeOrig = itrange;
		StringIteratorRange::ITERATOR_T &it = itrange.begin;

		size_t newlines = 0;
		while (it != itrange.end && std::isspace(*it))
		{
			newlines += isNewLine(*it);
			++it;
		}

		if (it == itrange.end)
			return ParseResult(ParseResult::Status::Invalid, COMMENT_RESULT(itrange, commentLength), nullptr, { itrange.shifted(newlines), "Expected name"});

		if (std::isdigit(*it))
			return ParseResult(ParseResult::Status::Invalid, COMMENT_RESULT(itrange, commentLength), nullptr, { itrange.shifted(newlines), "Names may not start with a digit" });

		for (; it != itrange.end; ++it)
		{
			char chr = *it;

			//if (std::isalnum(chr) || chr == ':')
			if (ParserNameSpecifier::validateChar(it, itrangeOrig))
				this->name.push_back(*it);
			else
				break;
		}

		if (this->name.empty())
			return ParseResult(ParseResult::Status::Invalid, COMMENT_RESULT(itrange, commentLength), nullptr, { itrange.shifted(newlines), "Expected name" });

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(it - this->name.length(), it));
	}

	bool ParserNameSpecifier::validateChar(std::string::const_iterator it, StringIteratorRange itrange)
	{
		if (std::isalnum(*it) || *it == '_')
			return true;

		// asdas:dasd:

		if (*it == ':')
		{
			if (it != itrange.begin)
			{
				if (*(it - 1) == ':')
					return true;
			}

			if (it != itrange.end - 1)
			{
				if (*(it + 1) == ':')
					return true;
			}
		}

		return false;
	}
}