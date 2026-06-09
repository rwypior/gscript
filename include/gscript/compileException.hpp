#ifndef _h_gscript_compileexception
#define _h_gscript_compileexception

#include <exception>
#include <string>
#include <sstream>

namespace gscript
{
	class CompileException : public std::exception
	{
	public:
		CompileException(const std::string msg)
			: std::exception()
			, msg(msg)
		{
		}

		virtual const char *what() const
		{
			return this->msg.c_str();
		}

	protected:
		std::string msg;
	};

	class VariableNotFoundException : public CompileException
	{
	public:
		VariableNotFoundException(const std::string& varname)
			: CompileException((std::stringstream() << "Variable \"" << varname << "\" was not found").str())
		{
		}
	};
}

#endif