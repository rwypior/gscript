#ifndef _h_gscript_extension_sample1
#define _h_gscript_extension_sample1

#include "script.hpp"
#include "scriptExtension.hpp"
#include "function.hpp"
#include "class.hpp"

#include <sys/stat.h>

// TODO - move this to separate project, tests maybe?

namespace gscript
{
	class SampleExtension : public ScriptExtension
	{
	public:
		class FuncMultiply : public ScriptExternFunction
		{
		public:
			FuncMultiply(ScriptScope &scope, const std::string &name)
				:ScriptExternFunction(
					scope,
					name,
					ScriptType::create(VALUE_TYPE_T::VT_INT, this->scope),
					PARAMS_T({
						FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_INT, scope)),
						FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_INT, scope))
						})
				)
			{
			}

			virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override
			{
				this->validateParams(c);

				const ScriptIntValue &a = static_cast<const ScriptIntValue&>(*c[0]);
				const ScriptIntValue &b = static_cast<const ScriptIntValue&>(*c[1]);

				ScriptIntValue *result = new ScriptIntValue(a.getValue() * b.getValue());

				return result;
			}
		};

		void run(Script &script)
		{
			script.getMainScope()->registerExternFunction(new FuncMultiply(*script.getMainScope(), "func_multiply"));
		}

		std::string getName() const override
		{
			return "sample";
		}
	};
}

#endif