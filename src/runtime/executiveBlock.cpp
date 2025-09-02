#include "runtime/executiveBlock.hpp"
#include "runtime/return.hpp"

namespace gscript
{
	ScriptExecutiveBlock::ScriptExecutiveBlock(std::vector<std::shared_ptr<ScriptCallable>>&& statements)
		: statements(std::move(statements))
	{
	}

	ScriptExecutiveBlock::ScriptExecutiveBlock(ScriptExecutiveBlock&& exeblock)
		: statements(std::move(exeblock.statements))
	{
	}

	ScriptValue* ScriptExecutiveBlock::execute()
	{
		for (auto& stmt : this->statements)
		{
			if (std::shared_ptr<ScriptReturn> ret = std::dynamic_pointer_cast<ScriptReturn>(stmt))
				return ret->run();

			stmt->run();
		}

		return SCR_NULL;
	}

	void ScriptExecutiveBlock::merge(ScriptExecutiveBlock&& block)
	{
		this->statements.insert(this->statements.end(), std::make_move_iterator(block.statements.begin()), std::make_move_iterator(block.statements.end()));
	}

	void ScriptExecutiveBlock::merge(std::unique_ptr<ScriptExecutiveBlock>&& block)
	{
		this->merge(std::move(*block));
	}

	const std::vector<std::shared_ptr<ScriptCallable>>& ScriptExecutiveBlock::getStatements()
	{
		return this->statements;
	}
}