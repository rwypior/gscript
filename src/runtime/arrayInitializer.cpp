#include "runtime/arrayInitializer.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptArrayInitializer::ScriptArrayInitializer(ScriptScope& scope, std::vector<std::unique_ptr<ScriptStatement>>&& statements)
		: ScriptCallable(scope)
		, statements(std::move(statements))
		, type(statements.empty() ? ScriptType::create(VALUE_TYPE_T::VT_VOID, scope) : statements.front()->getType())
	{
		ScriptType::create(VALUE_TYPE_T::VT_VOID, scope);
	}

	std::unique_ptr<ScriptValue> ScriptArrayInitializer::run(const CALLABLE_PARAMS_T &c)
	{
		std::vector<std::unique_ptr<ScriptValue>> vec;
		vec.reserve(this->statements.size());

		for (auto& stmt : this->statements)
		{
			vec.push_back(stmt->run());
		}

		return std::make_unique<ScriptArrayValue>(this->getType(), std::move(vec));
	}

	const std::shared_ptr<ScriptType> ScriptArrayInitializer::getType() const
	{
		return this->type;
	}
}