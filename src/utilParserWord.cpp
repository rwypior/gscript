#include "utilParserWord.hpp"

#include <string>

namespace gscript
{
	extern const std::string Util::Word::WORD_ANY = "";

	ParseResult Util::Word::parse(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult, bool allowSpaces)
	{
		int length = word.length();

		if (itrange.end - itrange.begin < length + 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		std::string buffer(length, '0');

		StringIteratorRange::ITERATOR_T it = itrange.begin;

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
				if (allowSpaces && buffer == word)
					break;

				if (it + 1 == itrange.end)
					break;
				char next = *(it + 1);
				if (!std::isalnum(next))
					break;

				return ParseResult(ParseResult::STATUS_T::S_FATAL);
			}
		}

		if (buffer == word)
		{
			StringIteratorRange::ITERATOR_T foundBegin = it + 1 - length; // * MSVC if this is it - length + 1 <- an error will be generated

			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(foundBegin, it + 1), std::move(subResult));
		}

		return ParseResult(ParseResult::STATUS_T::S_FATAL);
	}

	ParseResult Util::Word::parseUntil(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult, const std::string &allowed)
	{
		int length = word.length();

		if (itrange.end - itrange.begin < length + 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		char *buffer = new char[length];

		StringIteratorRange::ITERATOR_T it = itrange.begin;

		int i = 0;
		for (; it != itrange.end; ++it)
		{
			Util::Word::copy(buffer, StringIteratorRange(it, it + length));
			bool ok = strncmp(buffer, word.c_str(), length) == 0;

			if (ok)
			{
				delete[] buffer;
				return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(itrange.begin, it + 1), std::move(subResult));
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

	void Util::Word::copy(char *destination, StringIteratorRange itrange)
	{
		int i = 0;
		for (StringIteratorRange::ITERATOR_T it = itrange.begin; it != itrange.end; ++it, ++i)
		{
			destination[i] = *it;
		}
	}
}