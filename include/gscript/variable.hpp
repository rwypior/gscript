#ifndef _h_gscript_variable
#define _h_gscript_variable

#include "defs.hpp"

#include "lib.hpp"

#include <string>

namespace gscript
{
	class ScriptType;
	class ScriptValue;

	class ScriptVariable
	{
		friend class ScriptVariableResolv;

	protected:
		const std::string name;
		ScriptValue *value;
		const ScriptType *type;

		size_t internalPointer;

	public:
		SCRIPT_API ScriptVariable(const std::string &name, const ScriptType *type, ScriptValue *value, size_t internalPointer);
		virtual ~ScriptVariable() = default;

		ScriptValue *getValue() const
		{
			return this->value;
		}

		void setValue(ScriptValue *value)
		{
			//if (this->value)
				//delete this->value;

			this->value = value;
		}

		SCRIPT_API void init(ScriptValue *value);

		const std::string &getName() const
		{
			return this->name;
		}

		const ScriptType *getType() const
		{
			return this->type;
		}

		size_t getInternalPointer() const
		{
			return this->internalPointer;
		}
	};
}

#endif