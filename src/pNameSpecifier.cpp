#include "pNameSpecifier.hpp"
#include "pComment.hpp"

#include <iostream>
#include <cctype>

namespace gscript
{
	ParserNameSpecifier::ParserNameSpecifier(const std::string &name)
		:name(name)
	{ }

	ParseResult ParserNameSpecifier::parse(StringIteratorRange itrange)
	{
		std::string::iterator begin = itrange.begin;

		unsigned int commentLength = 0;
		COMMENT(itrange, itrange.begin, commentLength)

			if (itrange.end - itrange.begin < 1)
				return ParseResult(ParseResult::STATUS_T::S_FATAL, COMMENT_RESULT(itrange, commentLength));

		StringIteratorRange itrangeOrig = itrange;
		StringIteratorRange::ITERATOR_T &it = itrange.begin;

		while (std::isspace(*it))
		{
			++it;
		}

		if (std::isdigit(*it))
			return ParseResult(ParseResult::STATUS_T::S_FATAL, COMMENT_RESULT(itrange, commentLength));

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
			return ParseResult(ParseResult::STATUS_T::S_FATAL, COMMENT_RESULT(itrange, commentLength));

		return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(it - this->name.length(), it));
	}

	bool ParserNameSpecifier::validateChar(std::string::const_iterator it, StringIteratorRange itrange)
	{
		if (std::isalnum(*it))
			return true;

		if (*it == ':')
		{
			if (it != itrange.begin)
			{
				if (*(it - 1) == ':')
					return true;
			}

			if (it != itrange.end)
			{
				if (*(it + 1) == ':')
					return true;
			}
		}

		return false;
	}
}