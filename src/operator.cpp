#include "operator.hpp"
#include "pOperator.hpp"
#include "varRead.hpp"
#include "scopedCall.hpp"
#include "funcCall.hpp"

#include "compileException.hpp"

#include "debug.hpp"

#include <string>
#include <cassert>



#define OP_CONVERT(parserOperator) if (op->getChar() == parserOperator::KW_OPERATOR) return new parserOperator::SCRIPT_OPERATOR(scope);



namespace gscript
{
	ScriptOperator::ScriptOperator(ScriptScope &scope, OPERATOR_LINK_T linkage)
		:ScriptCallable(scope),
		linkage(linkage)
	{
	}

	const ScriptType *ScriptOperator::getType() const
	{
		return this->right ? this->right->getType() : this->left->getType();
	}

	void ScriptOperator::assignOperatorFunction()
	{
		VALUE_TYPE_T typeLeft = this->left ? this->left->getType()->getTypeDescriptor() : VALUE_TYPE_T::VT_VOID;
		VALUE_TYPE_T typeRight = this->right ? this->right->getType()->getTypeDescriptor() : VALUE_TYPE_T::VT_VOID;

		this->operatorFunction = OperatorFunctionFactory::getFunction(typeLeft, typeRight, this->getOperatorType());
	}

	bool ScriptOperator::needFactory() const
	{
		return true;
	}



	ScriptValue *ScriptOperatorMemberAccessor::run(const CALLABLE_PARAMS_T &c)
	{
		// 1. if this->right is scopedCall then provide it with classInstance from this->left
		// 2. this->right must be scoped call - always

		ScriptClassValue *scv = static_cast<ScriptClassValue*>(this->left->run());

		if (ScriptScopedCall *sc = dynamic_cast<ScriptScopedCall*>(this->right))
		{
			sc->setScope(scv->getValue());
		}

		if (ScriptFuncCall *fc = dynamic_cast<ScriptFuncCall*>(this->right))
		{
			fc->setInstance(scv);
		}

		return this->right->run(c);
	}

	const int ScriptOperatorMemberAccessor::getPrecedence() const
	{
		return 2;
	}

	const OPERATOR_TYPE_T ScriptOperatorMemberAccessor::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_MEMBER_ACCESSOR;
	}

	bool ScriptOperatorMemberAccessor::needFactory() const
	{
		return false;
	}



	ScriptValue *ScriptOperatorConditionalNull::run(const CALLABLE_PARAMS_T &c)
	{
		ScriptValue *v = this->left->run();
		return v->boolean().getValue() ? v : this->right->run();
	}

	const int ScriptOperatorConditionalNull::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorConditionalNull::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_CONDITIONAL_NULL;
	}

	bool ScriptOperatorConditionalNull::needFactory() const
	{
		return false;
	}



	ScriptValue *ScriptOperatorConditionalA::run(const CALLABLE_PARAMS_T &c)
	{
		return this->left->run();
	}

	const int ScriptOperatorConditionalA::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorConditionalA::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_CONDITIONAL_IF;
	}

	bool ScriptOperatorConditionalA::needFactory() const
	{
		return false;
	}



	ScriptValue *ScriptOperatorConditionalB::run(const CALLABLE_PARAMS_T &c)
	{
		auto x = this->left->run()->boolean().getValue() ? static_cast<ScriptOperatorConditionalA*>(this->left)->right->run() : this->right->run();
		return this->left->run()->boolean().getValue() ? static_cast<ScriptOperatorConditionalA*>(this->left)->right->run() : this->right->run();
	}

	const int ScriptOperatorConditionalB::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorConditionalB::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_CONDITIONAL_ELSE;
	}

	bool ScriptOperatorConditionalB::needFactory() const
	{
		return false;
	}

	void ScriptOperatorConditionalB::setup()
	{
		if (!dynamic_cast<ScriptOperatorConditionalA*>(this->left))
			throw CompileException("Invalid use of conditional operator");
	}



	ScriptValue *ScriptOperatorNegate::run(const CALLABLE_PARAMS_T &c)
	{
		return NULL;
	}

	const int ScriptOperatorNegate::getPrecedence() const
	{
		return 3;
	}

	const OPERATOR_TYPE_T ScriptOperatorNegate::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_NEGATE;
	}



	ScriptValue *ScriptOperatorAssign::run(const CALLABLE_PARAMS_T &c)
	{
		/*ScriptVarRead *varRead = dynamic_cast<ScriptVarRead*>(this->left);

		ScriptValue *rval = this->right->run();

		varRead->var->setValue(rval);

		return NULL;*/

		if (ScriptOperatorMemberAccessor *memberAccessor = dynamic_cast<ScriptOperatorMemberAccessor*>(this->left))
		{
			memberAccessor->run();
		}

		ScriptValue *rval = this->right->run();

		//this->varRead->var->setValue(rval);
		//this->varRead->var->get()->setValue(rval);
		auto x = this->varRead->var->get();
		this->varRead->var->get()->getValue()->assign(*rval);

		return NULL;
	}

	void ScriptOperatorAssign::setup()
	{
		this->varRead = dynamic_cast<ScriptVarRead*>(this->left);

		if (!this->varRead)
		{
			if (ScriptOperatorMemberAccessor *memberAccessor = dynamic_cast<ScriptOperatorMemberAccessor*>(this->left))
			{
				this->varRead = dynamic_cast<ScriptVarRead*>(memberAccessor->right);
			}
			else
				throw CompileException("Cannot assign value to non-variable");
		}
	}

	const int ScriptOperatorAssign::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorAssign::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_ASSIGN;
	}

	bool ScriptOperatorAssign::needFactory() const
	{
		return false;
	}



	ScriptValue *ScriptOperatorEquals::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorEquals::getPrecedence() const
	{
		return 10;
	}

	const OPERATOR_TYPE_T ScriptOperatorEquals::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_EQUALS;
	}



	ScriptValue *ScriptOperatorGreaterThan::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorGreaterThan::getPrecedence() const
	{
		return 9;
	}

	const OPERATOR_TYPE_T ScriptOperatorGreaterThan::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_GREATER_THAN;
	}



	ScriptValue *ScriptOperatorGreaterThanOrEqual::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorGreaterThanOrEqual::getPrecedence() const
	{
		return 9;
	}

	const OPERATOR_TYPE_T ScriptOperatorGreaterThanOrEqual::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_GREATER_THAN_OR_EQUAL;
	}



	ScriptValue *ScriptOperatorLessThan::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorLessThan::getPrecedence() const
	{
		return 9;
	}

	const OPERATOR_TYPE_T ScriptOperatorLessThan::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_LESSER_THAN;
	}



	ScriptValue *ScriptOperatorLessThanOrEqual::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorLessThanOrEqual::getPrecedence() const
	{
		return 9;
	}

	const OPERATOR_TYPE_T ScriptOperatorLessThanOrEqual::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_LESSER_THAN_OR_EQUAL;
	}



	ScriptValue *ScriptOperatorAdd::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorAdd::getPrecedence() const
	{
		return 6;
	}

	const OPERATOR_TYPE_T ScriptOperatorAdd::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_ADD;
	}



	ScriptValue *ScriptOperatorAddTo::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorAddTo::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorAddTo::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_ADD_TO;
	}



	ScriptValue *ScriptOperatorSubtract::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorSubtract::getPrecedence() const
	{
		return 6;
	}

	const OPERATOR_TYPE_T ScriptOperatorSubtract::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_SUBTRACT;
	}



	ScriptValue *ScriptOperatorSubtractFrom::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorSubtractFrom::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorSubtractFrom::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_SUBTRACT_FROM;
	}



	ScriptValue *ScriptOperatorMultiply::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorMultiply::getPrecedence() const
	{
		return 5;
	}

	const OPERATOR_TYPE_T ScriptOperatorMultiply::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_MULTIPLY;
	}



	ScriptValue *ScriptOperatorMultiplyBy::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorMultiplyBy::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorMultiplyBy::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_MULTIPLY_BY;
	}



	ScriptValue *ScriptOperatorDivide::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorDivide::getPrecedence() const
	{
		return 5;
	}

	const OPERATOR_TYPE_T ScriptOperatorDivide::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_DIVIDE;
	}



	ScriptValue *ScriptOperatorDivideBy::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorDivideBy::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorDivideBy::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_DIVIDE_BY;
	}



	ScriptValue *ScriptOperatorIncrement::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorIncrement::getPrecedence() const
	{
		return 2;
	}

	const OPERATOR_TYPE_T ScriptOperatorIncrement::getOperatorType() const
	{
		return this->left ? OPERATOR_TYPE_T::OT_POST_INCREMENT : OPERATOR_TYPE_T::OT_PRE_INCREMENT;
	}



	ScriptValue *ScriptOperatorDecrement::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left, this->right);
	}

	const int ScriptOperatorDecrement::getPrecedence() const
	{
		return 2;
	}

	const OPERATOR_TYPE_T ScriptOperatorDecrement::getOperatorType() const
	{
		return this->left ? OPERATOR_TYPE_T::OT_POST_DECREMENT : OPERATOR_TYPE_T::OT_PRE_DECREMENT;
	}
}