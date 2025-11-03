#ifndef _h_gscript_parser_word
#define _h_gscript_parser_word

#include "parser/pEntity.hpp"
#include "IteratorRange.hpp"

#include <string>
#include <memory>
#include <functional>

namespace gscript
{
	class ParseResult;

	/// Holds a set of utility functions for parsing will words
	namespace ParserWord
	{
		bool parsePredStrict(StringIteratorRange::ITERATOR_T it, StringIteratorRange::ITERATOR_T end, const std::string& buffer, const std::string& word);
		bool parsePredExact(StringIteratorRange::ITERATOR_T it, StringIteratorRange::ITERATOR_T end, const std::string& buffer, const std::string& word);

		/// Predicate function for parse function - stops parsing when returning true
		/// First argument - iterator to current parsing position
		/// Second argument - iterator to the end of parsing range
		/// Third argument - currently buffered word
		/// Fourth argument - searched word
		using parsePred = bool(StringIteratorRange::ITERATOR_T, StringIteratorRange::ITERATOR_T, const std::string&, const std::string&);

		/// Parses given iterator range for 'word', ignoring whitespaces at the beginning, and stops when 'pred' returns true
		/// fails when encounters non-alphanumeric word
		/// Default 'prod' function stops at the end of given range.
		/// fails when encounters non-alphanumeric word
		/// Returns location of 'word'
		ParseResult parse(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult = nullptr, std::function<parsePred> pred = parsePredStrict);

		/// Parses given iterator range for 'word', ignoring whitespaces at the beginning, and stops until given 'word' is
		/// encountering, ignoring anything else after the 'word'
		/// Fails when encounters non-alphanumeric word
		/// Returns location of 'word'
		ParseResult parseExact(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult = nullptr);

		/// Parses given iterator range until encountering 'word', returns iterator range from the begin until the begin
		/// of encountered 'word'
		ParseResult parseUntil(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult = nullptr, const std::string &allowed = "");

		/// Copy characters in the 'itrange' range into the 'destination' string
		void copy(char *destination, StringIteratorRange itrange);
	};
}

#endif