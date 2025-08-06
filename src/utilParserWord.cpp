#include "utilParserWord.hpp"

#include <iostream>
#include <cctype>
#include <cassert>

namespace gscript
{
	extern const std::string Util::Word::WORD_ANY = "";

	ParseResult Util::Word::parse(ParserEntity::StringIteratorRange itrange, const std::string &word, void *subResult, bool allowSpaces)
	{
		//if (word == ":")
		//	assert(!"DEBUG");

		int length = word.length();

		if (itrange.end - itrange.begin < length + 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		char *buffer = new char[length];

		ParserEntity::StringIteratorRange::ITERATOR_T it = itrange.begin;

		while (std::isspace(*it))
		{
			++it;
		}

		if (std::isdigit(*it))
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		int i = 0;
		for (; it != itrange.end; ++it)
		{
			char chr = *it;
			buffer[i++] = chr;

			if (i == length)
			{
				if (allowSpaces && strncmp(buffer, word.c_str(), length) == 0)
					break;

				if (it + 1 == itrange.end)
					break;
				char next = *(it + 1);
				if (!std::isalnum(next))
					break;

				delete[] buffer;
				return ParseResult(ParseResult::STATUS_T::S_FATAL);
			}
		}

		if (strncmp(buffer, word.c_str(), length) == 0)
		{
			ParserEntity::StringIteratorRange::ITERATOR_T foundBegin = it + 1 - length; // * MSVC if this is it - length + 1 <- an error will be generated
			delete[] buffer;

			return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(foundBegin, it + 1), subResult);
		}

		delete[] buffer;
		return ParseResult(ParseResult::STATUS_T::S_FATAL);
	}

	ParseResult Util::Word::parseUntil(ParserEntity::StringIteratorRange itrange, const std::string &word, void *subResult, const std::string &allowed)
	{
		int length = word.length();

		if (itrange.end - itrange.begin < length + 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		char *buffer = new char[length];

		ParserEntity::StringIteratorRange::ITERATOR_T it = itrange.begin;

		int i = 0;
		for (; it != itrange.end; ++it)
		{
			Util::Word::copy(buffer, ParserEntity::StringIteratorRange(it, it + length));
			bool ok = strncmp(buffer, word.c_str(), length) == 0;

			if (ok)
			{
				delete[] buffer;
				return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(itrange.begin, it + 1), subResult);
			}
			else if (!allowed.empty())
			{
				char chr = *it;

				if (allowed.find(chr) == std::string::npos)
					return ParseResult(ParseResult::STATUS_T::S_FATAL);
			}
		}

		delete[] buffer;
		return ParseResult(ParseResult::STATUS_T::S_FATAL);
	}

	void Util::Word::copy(char *destination, ParserEntity::StringIteratorRange itrange)
	{
		int i = 0;
		for (ParserEntity::StringIteratorRange::ITERATOR_T it = itrange.begin; it != itrange.end; ++it, ++i)
		{
			destination[i] = *it;
		}
	}
}