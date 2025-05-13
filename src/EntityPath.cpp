#include "EntityPath.hpp"

namespace gscript
{
	const char *EntityPath::KW_MEMBER_ACCESSOR = ".";
	const char *EntityPath::KW_SCOPE_ACCESSOR = "::";

	EntityPath::EntityPath(const std::string &path)
		:path(path)
	{
	}

	bool EntityPath::empty() const
	{
		return this->path.empty();
	}

	size_t EntityPath::length() const
	{
		return this->path.length();
	}

	void EntityPath::push_back(char c)
	{
		this->path.push_back(c);
	}

	EntityPath::scope_iterator EntityPath::createScopeIterator() const
	{
		return scope_iterator(EntityPath::KW_SCOPE_ACCESSOR, this->path);
	}

	EntityPath::operator std::string() const
	{
		return this->path;
	}

	bool EntityPath::isScoped() const
	{
		return this->path.find_first_of(EntityPath::KW_SCOPE_ACCESSOR) != std::string::npos;
	}

	const std::string &EntityPath::getString() const
	{
		return this->path;
	}
}