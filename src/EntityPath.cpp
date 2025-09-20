#include "EntityPath.hpp"

namespace gscript
{
	EntityPath::EntityPath(const std::string &path)
		: path(path)
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
		return scope_iterator(EntityPath::keycharScopeAccessor, this->path);
	}

	EntityPath::operator std::string() const
	{
		return this->path;
	}

	bool EntityPath::isScoped() const
	{
		return this->path.find_first_of(EntityPath::keycharScopeAccessor) != std::string::npos;
	}

	const std::string &EntityPath::getString() const
	{
		return this->path;
	}

	bool EntityPath::operator==(const std::string& str) const
	{
		return this->getString() == str;
	}
}