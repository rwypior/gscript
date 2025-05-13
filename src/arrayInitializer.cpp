#include "arrayInitializer.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptArrayInitializer::ScriptArrayInitializer(ScriptScope &scope, ParserArrayInitializer &arrayInitializer)
		:ScriptCallable(scope)
	{
		const ScriptType *prevType = NULL;

		for (ParserCallArglist::STATEMENT_CONTAINER_T::iterator it = arrayInitializer.arglist.parameters.begin(); it != arrayInitializer.arglist.parameters.end(); ++it)
		{
			this->statements.push_back(new ScriptStatement(scope, **it));
			const ScriptType *type = this->statements.back()->getType();

			if (prevType && !prevType->equals(*type))
				throw CompileException("Array must contain the same types");

			prevType = type;
		}

		this->type = new ScriptArrayType(prevType ? prevType : ScriptType::create(VALUE_TYPE_T::VT_NULL, scope));
	}

	ScriptValue *ScriptArrayInitializer::run(const CALLABLE_PARAMS_T &c)
	{
		ScriptValue **v = new ScriptValue*[this->statements.size()];

		size_t i = 0;
		for (
			STATEMENT_CONTAINER_T::iterator it = this->statements.begin();
			it != this->statements.end();
			++it, ++i)
		{
			v[i] = (*it)->run();
		}

		return new ScriptArrayValue(static_cast<const ScriptArrayType*>(this->getType()), v);
	}

	const ScriptType *ScriptArrayInitializer::getType() const
	{
		return this->type;
	}
}