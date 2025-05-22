#ifndef _h_gscript_function_path
#define _h_gscript_function_path

#include <string>
#include <vector>

#include "defs.hpp"

namespace gscript
{
	class EntityPath
	{
	public:
		static const char *KW_MEMBER_ACCESSOR;
		static const char *KW_SCOPE_ACCESSOR;

		class scope_iterator : public std::iterator<std::forward_iterator_tag, std::string>
		{
		protected:
			std::string::const_iterator it;
			std::string::const_iterator it2;
			const std::string &target;
			const char *separator;

			void update()
			{
				auto x = this->it - this->target.begin();
				size_t res = this->target.find_first_of(this->separator, this->it - this->target.begin());
				if (res == std::string::npos)
					it2 = this->target.end();
				else
					it2 = this->target.begin() + res;
			}

		public:
			const std::string operator*() const
			{
				return std::string(it, it2);
			}

			scope_iterator& operator++()
			{
				if (this->it2 == this->target.end())
					this->it = it2;
				else
					this->it = this->it2 + 2;
				this->update();

				return *this;
			}

			bool operator ==(scope_iterator const &b) const
			{
				return this->it == b.it;
			}

			bool operator ==(std::string::iterator const &b) const
			{
				return this->it == b;
			}

			bool operator ==(std::string::const_iterator const &b) const
			{
				return this->it == b;
			}

			bool operator !=(scope_iterator const &b) const
			{
				return !(*this == b);
			}

			bool operator !=(std::string::iterator const &b) const
			{
				return !(*this == b);
			}

			bool operator !=(std::string::const_iterator const &b) const
			{
				return !(*this == b);
			}

			bool last()
			{
				return it2 == this->target.end();
			}

			scope_iterator(const char *separator, const std::string &target)
				:target(target),
				it(target.begin()),
				separator(separator)
			{
				this->update();
			}
		};

	private:
		std::string path;

	public:
		EntityPath() = default;
		EntityPath(const std::string &path);

		bool isScoped() const;

		scope_iterator createScopeIterator() const;

		bool empty() const;
		size_t length() const;
		void push_back(char c);

		operator std::string() const;
		const std::string &getString() const;
	};
}

#endif