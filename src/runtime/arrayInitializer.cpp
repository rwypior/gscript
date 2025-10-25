#include "runtime/arrayInitializer.hpp"
#include "compileException.hpp"
#include "util.hpp"

namespace gscript
{
	ScriptArrayInitializer::ScriptArrayInitializer(const ScriptArrayInitializer& b)
		: type(b.type->clone())
		, statements(cloneVector(b.statements))
	{
	}

	ScriptArrayInitializer::ScriptArrayInitializer(ScriptScope& scope, std::vector<std::unique_ptr<ScriptStatement>>&& statements)
		//: ScriptCallable(scope)
		: statements(std::move(statements))
		, type(statements.empty() ? ScriptType::create(ValueType::Void, scope) : statements.front()->getType())
	{
		ScriptType::create(ValueType::Void, scope);
	}

	std::unique_ptr<ScriptCallable> ScriptArrayInitializer::clone()
	{
		return std::make_unique<ScriptArrayInitializer>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptArrayInitializer::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		std::vector<std::unique_ptr<ScriptValue>> vec;
		vec.reserve(this->statements.size());

		for (auto& stmt : this->statements)
		{
			vec.push_back(stmt->run(scope));
		}

		return std::make_unique<ScriptArrayValue>(this->getType(), std::move(vec));
	}

	const std::shared_ptr<ScriptType> ScriptArrayInitializer::getType() const
	{
		return this->type;
	}
}