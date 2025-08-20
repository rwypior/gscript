//#include "runtime/runnable.hpp"
//#include "runtime/statement.hpp"
//#include "runtime/varDeclaration.hpp"
//#include "runtime/return.hpp"
//#include "runtime/scriptValue.hpp"
//#include "runtime/if.hpp"
//#include "runtime/while.hpp"
//#include "runtime/for.hpp"
//
//namespace gscript
//{
//	ScriptRunnable::ScriptRunnable(ScriptScope *parentScope, std::vector<std::shared_ptr<ScriptCallable>>&& statements)
//		: ScriptScope(parentScope)
//		, statements(statements)
//	{ }
//
//	/*void ScriptRunnable::setup(const ParserBlockBody &pbody)
//	{
//		for (std::vector<std::shared_ptr<ParserEntity>>::const_iterator it = pbody.statements.begin(); it != pbody.statements.end(); ++it)
//		{
//			if (std::shared_ptr<ParserVarDeclaration> statement = std::dynamic_pointer_cast<ParserVarDeclaration>(*it))
//			{
//				ScriptVariable &svar = this->registerVariable(statement->name, ScriptType::create(statement->type, *this), NULL);
//				std::shared_ptr<ScriptCallable> svd(new ScriptVarDeclaration(*this, svar, ScriptStatement(*this, statement->value)));
//				this->statements.push_back(std::move(svd));
//			}
//			else if (std::shared_ptr<ParserStatement> statement = std::dynamic_pointer_cast<ParserStatement>(*it))
//			{
//				std::unique_ptr<ScriptCallable> sstat(new ScriptStatement(*this, *statement));
//				this->statements.push_back(std::move(sstat));
//			}
//			else if (std::shared_ptr<ParserReturn> statement = std::dynamic_pointer_cast<ParserReturn>(*it))
//			{
//				std::unique_ptr<ScriptCallable> sstat(new ScriptReturn(*this, ScriptStatement(*this, statement->value)));
//				this->statements.push_back(std::move(sstat));
//			}
//			else if (std::shared_ptr<ParserIf> statement = std::dynamic_pointer_cast<ParserIf>(*it))
//			{
//				ScriptIf *sif = new ScriptIf(*this, *statement, *this);
//
//				std::unique_ptr<ScriptCallable> sstat(sif);
//				sif->setup(statement->body.body);
//				this->statements.push_back(std::move(sstat));
//			}
//			else if (std::shared_ptr<ParserWhile> statement = std::dynamic_pointer_cast<ParserWhile>(*it))
//			{
//				ScriptWhile *swhile = new ScriptWhile(*this, *statement);
//				std::unique_ptr<ScriptCallable> sstat(swhile);
//				swhile->setup(statement->body.body);
//				this->statements.push_back(std::move(sstat));
//			}
//			else if (std::shared_ptr<ParserFor> statement = std::dynamic_pointer_cast<ParserFor>(*it))
//			{
//				ScriptFor *sfor = new ScriptFor(*this, *statement);
//				std::unique_ptr<ScriptCallable> sstat(sfor);
//				sfor->setup(statement->body.body);
//				this->statements.push_back(std::move(sstat));
//			}
//		}
//	}*/
//
//	/*void ScriptRunnable::setupPrototypes(ScriptScope& scope)
//	{
//		for (auto& stmt : this->statements)
//		{			
//			if (auto& proto = std::dynamic_pointer_cast<Prototype>(stmt))
//			{
//				stmt = proto->setup(scope);
//			}
//		}
//	}*/
//
//	ScriptValue *ScriptRunnable::run()
//	{
//		for (auto& stmt : this->statements)
//		{
//			if (std::shared_ptr<ScriptReturn> ret = std::dynamic_pointer_cast<ScriptReturn>(stmt))
//				return ret->run();
//
//			stmt->run();
//		}
//
//		return SCR_NULL;
//	}
//}