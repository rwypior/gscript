//#include "runtime/conditionalOperator.hpp"
//#include "runtime/function.hpp"
//#include "defs.hpp"
//
//#include "compileException.hpp"
//
//#include <iostream>
//
//namespace gscript
//{
//	ScriptConditionalOperator::ScriptConditionalOperator(ScriptScope& scope, ParserConditionalOperator* cond)
//		:ScriptCallable(scope),
//		cond(new ScriptStatement(scope, cond->condition)),
//		ontrue(cond->ontrue ? new ScriptStatement(scope, cond->ontrue) : new ScriptStatement(scope, cond->condition)),
//		onfalse(new ScriptStatement(scope, cond->onfalse))
//	{
//	}
//
//	ScriptConditionalOperator::ScriptConditionalOperator(ScriptScope& scope, ScriptStatement* cond, ScriptStatement* ontrue, ScriptStatement* onfalse)
//		:ScriptCallable(scope),
//		cond(cond),
//		ontrue(ontrue),
//		onfalse(onfalse)
//	{
//	}
//
//	ScriptConditionalOperator::ScriptConditionalOperator(ScriptScope& scope, ScriptStatement* cond, ScriptStatement* onfalse)
//		:ScriptCallable(scope),
//		cond(cond),
//		ontrue(cond),
//		onfalse(onfalse)
//	{
//	}
//
//	ScriptValue* ScriptConditionalOperator::run(const CALLABLE_PARAMS_T& c)
//	{
//		return this->cond->run()->boolean().getValue() ? this->ontrue->run() : this->onfalse->run();
//	}
//
//	const ScriptType* ScriptConditionalOperator::getType() const
//	{
//		return this->ontrue->getType();
//	}
//}