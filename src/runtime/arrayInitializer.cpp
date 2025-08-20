#include "runtime/arrayInitializer.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptArrayInitializer::ScriptArrayInitializer(ScriptScope& scope, std::vector<std::unique_ptr<ScriptStatement>>&& statements)
		: ScriptCallable(scope)
		, statements(std::move(statements))
		, type(statements.empty() ? ScriptType::create(VALUE_TYPE_T::VT_VOID, scope) : statements.front()->getType())
	{
	}

	ScriptValue *ScriptArrayInitializer::run(const CALLABLE_PARAMS_T &c)
	{
		ScriptValue **v = new ScriptValue*[this->statements.size()];

		size_t i = 0;
		for (auto& stmt : this->statements)
		{
			v[i++] = stmt->run();
		}

		return new ScriptArrayValue(static_cast<const ScriptArrayType*>(this->getType()), v);
	}

	const ScriptType *ScriptArrayInitializer::getType() const
	{
		return this->type;
	}
}