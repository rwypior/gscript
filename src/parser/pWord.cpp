#include "parser/pWord.hpp"

#include <string>

namespace
{
	std::string quoted(const std::string& word)
	{
		return "\"" + word + "\"";
	}

	std::string errmsg(const std::string& expected, const std::string& got = "")
	{
		std::string msg = "Expected \"" + expected + "\"";
		if (!expected.empty())
			msg += ", got " + got;
		return msg;
	}
}

namespace gscript
{
	extern const std::string ParserWord::WORD_ANY = "";

	ParseResult ParserWord::parse(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult/*, bool allowSpaces*/)
	{
		int length = word.length();

		//if (itrange.end - itrange.begin < length + 1)
		if (itrange.end - itrange.begin < length)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, { itrange, errmsg(word, "empty statement") });

		std::string buffer(length, '0');

		StringIteratorRange::ITERATOR_T it = itrange.begin;
		size_t newlines = 0;

		while (it != itrange.end && std::isspace(*it))
		{
			newlines += std::isspace(*it);
			++it;
		}

		if (it == itrange.end)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, { itrange.shifted(newlines), errmsg(word, "empty statement") });

		if (std::isdigit(*it))
			return ParseResult(ParseResult::STATUS_T::S_FATAL, { itrange.shifted(newlines), "Expected alphanumeric word, got number" });

		int i = 0;
		for (; it != itrange.end; ++it)
		{
			char chr = *it;
			buffer[i++] = chr;

			if (i == length)
			{
				/*if (allowSpaces && buffer == word)
					break;*/

				if (it + 1 == itrange.end)
					break;
				char next = *(it + 1);
				if (!std::isalnum(next))
					break;

				return ParseResult(ParseResult::STATUS_T::S_FATAL, { itrange.shifted(newlines), "Expected alphanumeric word, got special character" });
			}
		}

		if (buffer == word)
		{
			StringIteratorRange::ITERATOR_T foundBegin = it + 1 - length;

			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(foundBegin, it + 1), std::move(subResult));
		}

		return ParseResult(ParseResult::STATUS_T::S_FATAL, {itrange.shifted(newlines), errmsg(word, quoted(buffer)) });
	}

	ParseResult ParserWord::parseUntil(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult, const std::string &allowed)
	{
		int length = word.length();

		if (itrange.end - itrange.begin < length + 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		std::string buffer(length, '0');

		StringIteratorRange::ITERATOR_T it = itrange.begin;

		size_t newlines = 0;
		int i = 0;
		for (; it != itrange.end; ++it)
		{
			buffer = std::string(it, it + length);
			bool ok = buffer == word;

			if (ok)
			{
				//return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(itrange.begin, it + 1), std::move(subResult));
				return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(itrange.begin, it), std::move(subResult));
			}
			else if (!allowed.empty())
			{
				char chr = *it;
				newlines += std::isspace(chr);

				if (allowed.find(chr) == std::string::npos)
					return ParseResult(ParseResult::STATUS_T::S_FATAL, {itrange.shifted(newlines), "Expected one of \"" + allowed + "\", got \"" + chr + "\""});
			}
		}

		return ParseResult(ParseResult::STATUS_T::S_FATAL, {itrange.shifted(newlines), "Expected \"" + word + "\", got \"" + buffer + "\"" });
	}

	void ParserWord::copy(char *destination, StringIteratorRange itrange)
	{
		int i = 0;
		for (StringIteratorRange::ITERATOR_T it = itrange.begin; it != itrange.end; ++it, ++i)
		{
			destination[i] = *it;
		}
	}
}