#ifndef _hpp_lib_path
#define _hpp_lib_path

#include <string>
#include <vector>
#include <algorithm>

#if defined(_WIN32) || defined(_WIN64)
	#define OPSYSTEM_WINDOWS
#elif defined(__linux)
	#define OPSYSTEM_LINUX
#endif

#ifdef OPSYSTEM_WINDOWS
	#include <windows.h>

	#define DIRECTORY_SEPARATOR '\\'
	#define DIRECTORY_SEPARATOR_REPLACEMENT '/'
	#define DIRECTORY_CURRENT ".\\"
	#define DIRECTORY_UP "..\\"
#else
	#include <unistd.h>

	#define DIRECTORY_SEPARATOR '/'
	#define DIRECTORY_SEPARATOR_REPLACEMENT '\\'
	#define DIRECTORY_CURRENT "./"
	#define DIRECTORY_UP "../"
#endif

namespace gscript
{
	namespace TextUtils
	{
		const std::string TRIM_DEFAULT_CHARS = " ";

		std::string ltrim(std::string s, const std::string &chars = TRIM_DEFAULT_CHARS)
		{
			size_t startpos = s.find_first_not_of(chars);
			if (std::string::npos != startpos)
				s = s.substr(startpos);
			return s;
		}

		std::string rtrim(std::string s, const std::string &chars = TRIM_DEFAULT_CHARS)
		{
			size_t endpos = s.find_last_not_of(chars);
			if (std::string::npos != endpos)
				s = s.substr(0, endpos + 1);
			return s;
		}

		std::string trim(const std::string &s, const std::string &chars = TRIM_DEFAULT_CHARS)
		{
			return ltrim(rtrim(s, chars), chars);
		}
	};

	class Path
	{
	protected:
		std::string path;

		void normalize()
		{
			if (this->path.length())
			{
				this->path = Path::fixDirectorySeparators(this->path);

				if (!this->isFile() && this->path.back() != DIRECTORY_SEPARATOR)
					this->path.push_back(DIRECTORY_SEPARATOR);
			}
		}

	public:
		static constexpr bool duplicatedSeparators(char lhs, char rhs)
		{
			return (lhs == rhs) && (lhs == DIRECTORY_SEPARATOR);
		}

		void resolveRelativeness()
		{
			if (!this->isRelative())
				return;

			std::vector<std::string> parts = this->getParts();
			std::vector<std::string> buffer;

			for (unsigned int i = 0; i < parts.size(); ++i)
			{
				std::string &part = parts[i];

				if (part + DIRECTORY_SEPARATOR == DIRECTORY_CURRENT)
					continue;

				if (part + DIRECTORY_SEPARATOR == DIRECTORY_UP)
				{
					if (buffer.size() > 0)
						buffer.pop_back();
					continue;
				}

				if (part.length() > 0)
					buffer.push_back(part);
			}

			this->path.clear();
			for (unsigned int i = 0; i < buffer.size(); ++i)
			{
				this->path += buffer[i];

				if (i + 1 < buffer.size())
					this->path += DIRECTORY_SEPARATOR;
			}
		}

		Path(const Path &path)
			:path(path.path)
		{
		}

		Path(const std::string &path = "")
			: path(path)
		{
			this->normalize();
		}

		Path(const char *path)
			: Path(std::string(path))
		{ }

		const std::string &toString() const
		{
			return this->path;
		}

		operator std::string() const
		{
			return this->path;
		}

		Path &addFragment(const std::string &fragment)
		{
			std::string trimmed = TextUtils::trim(Path::fixDirectorySeparators(fragment), std::string(1, DIRECTORY_SEPARATOR));

			trimmed = DIRECTORY_SEPARATOR + trimmed;

			this->path += trimmed;

			if (!this->isFile())
				this->path.push_back(DIRECTORY_SEPARATOR);

			this->resolveRelativeness();

			return *this;
		}

		Path &addFragment(const Path &fragment)
		{
			return this->addFragment(static_cast<const std::string&>(fragment));
		}

		void removeLastFragment()
		{
			size_t pos = TextUtils::rtrim(this->path, std::string(1, DIRECTORY_SEPARATOR)).find_last_of(DIRECTORY_SEPARATOR);

			if (pos == std::string::npos)
				this->path = "";
			else
			{
				this->path = this->path.substr(0, pos);
				this->normalize();
			}
		}


		std::string getLastFragment(bool stripExtension = false) const
		{
			int lastSlash = this->path.find_last_of(DIRECTORY_SEPARATOR);

			if (lastSlash == std::string::npos)
				return this->path;

			std::string lastFragment = this->path.substr(lastSlash + 1);

			if (stripExtension)
			{
				int lastDot = lastFragment.find_last_of('.');
				if (lastDot != std::string::npos)
					lastFragment = lastFragment.substr(0, lastDot);
			}

			return lastFragment;
		}

		std::string getExtension() const
		{
			std::string lastFragment = this->getLastFragment();
			int lastDot = lastFragment.find_last_of('.');

			if (lastDot == std::string::npos)
				return "";

			return lastFragment.substr(lastDot + 1);
		}

		std::vector<std::string> getParts() const
		{
			std::vector<std::string> result;
			result.emplace_back();

			for (unsigned int i = 0; i < this->path.length(); ++i)
			{
				char c = Path::fixSingleSeparator(this->path[i]);

				if (c == DIRECTORY_SEPARATOR)
				{
					result.emplace_back();
					continue;
				}

				result.back().push_back(c);
			}

			return result;
		}

		bool isFile() const
		{
			return this->getLastFragment().find('.') != std::string::npos;
		}

		bool exists() const
		{
			struct stat buffer;
			return (stat(this->path.c_str(), &buffer) == 0);
		}

		bool Path::isAbsolute() const
		{
#ifdef OPSYSTEM_WINDOWS
			return this->path.find(':') != std::string::npos;
#else
			return this->path[0] == Path::DIRECTORY_SEPARATOR;
#endif
		}

		bool isRelative() const
		{
			return this->path.find_first_of("./") != std::string::npos;
		}

		Path getDirectory() const
		{
			Path result(*this);

			if (!this->isFile())
				return result;

			result.removeLastFragment();
			return result;
		}

		Path clone(const Path &add = "") const
		{
			Path res(*this);
			if (add.length())
				res += add;
			return res;
		}

		size_t length() const
		{
			return this->path.length();
		}

		Path operator+(const Path &add) const
		{
			Path p = *this;
			return p += add;
		}

		Path &operator+=(const Path &add)
		{
			return this->addFragment(add);
		}

		Path getProgramPath()
		{
#ifdef OPSYSTEM_WINDOWS
			HMODULE mod = GetModuleHandle(NULL);
			if (mod == NULL)
			{
				std::cout << "Program path could not be obtained\n";
				exit(EXIT_FAILURE);
			}

			char path[MAX_PATH];
			GetModuleFileNameA(mod, path, MAX_PATH);

			return Path(path);
#else
			char path[PATH_MAX];
			char buf[64];
			sprintf(buf, "/proc/%d/exe\0", getpid());

			int chars = readlink(buf, path, PATH_MAX);

			if (chars == -1)
			{
				std::cout << "Program path could not be obtained\n";
				exit(EXIT_FAILURE);
			}

			path[chars] = 0;
			return Path(path);
#endif
		}

		std::string fixDirectorySeparators(std::string path)
		{
			std::replace(path.begin(), path.end(), DIRECTORY_SEPARATOR_REPLACEMENT, DIRECTORY_SEPARATOR);

			std::string::iterator new_end = std::unique(path.begin(), path.end(), duplicatedSeparators);
			path.erase(new_end, path.end());

			return path;
		}

		char fixSingleSeparator(char separator) const
		{
			if (separator == DIRECTORY_SEPARATOR_REPLACEMENT)
				return DIRECTORY_SEPARATOR;

			return separator;
		}
	};
}

#endif