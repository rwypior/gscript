#ifndef _h_gscript_stringutils
#define _h_gscript_stringutils

#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>
#include <sstream>

namespace gscript
{
	// trim from start (in place)
	static inline void ltrim(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s)
	{
		ltrim(s);
		rtrim(s);
	}

	// trim from start (copying)
	static inline std::string ltrim_copy(std::string s)
	{
		ltrim(s);
		return s;
	}

	// trim from end (copying)
	static inline std::string rtrim_copy(std::string s)
	{
		rtrim(s);
		return s;
	}

	// trim from both ends (copying)
	static inline std::string trim_copy(std::string s)
	{
		trim(s);
		return s;
	}

	// find position of last non-whitespace
	static inline std::string::const_iterator find_last_nonwhitespace(const std::string &s)
	{
		return std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base();
	}

	static inline bool isNewLine(char c)
	{
		return c == '\n';
	}

	static inline size_t skipWhitespaces(std::string::const_iterator& it, std::string::const_iterator end)
	{
		size_t newlines = 0;
		while (it != end && std::isspace(*it))
		{
			newlines += isNewLine(*it);
			it++;
		}
		return newlines;
	}

	static inline std::string getCharsUntil(std::string::const_iterator& it, std::string::const_iterator end, char until)
	{
		std::string res;
		while (it != end && *it != until)
		{
			res += *it;
			it++;
		}
		return res;
	}
}

#endif