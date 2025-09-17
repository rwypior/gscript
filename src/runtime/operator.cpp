#include "runtime/operator.hpp"
#include "runtime/varRead.hpp"
#include "runtime/scopedCall.hpp"
#include "runtime/funcCall.hpp"
#include "compileException.hpp"
#include "debug.hpp"

#include <string>
#include <cassert>

#define OP_CONVERT(parserOperator) if (op->getChar() == parserOperator::KW_OPERATOR) return new parserOperator::SCRIPT_OPERATOR(scope);

namespace gscript
{
	std::unordered_map<OPERATOR_TYPE_T, std::string> ScriptOperator::opmap = {
		{ OPERATOR_TYPE_T::OT_INVALID, "Invalid" },
		{ OPERATOR_TYPE_T::OT_MEMBER_ACCESSOR, "Member accessor" },
		{ OPERATOR_TYPE_T::OT_ADD, "Add" },
		{ OPERATOR_TYPE_T::OT_ADD_TO, "Add to" },
		{ OPERATOR_TYPE_T::OT_SUBTRACT, "Subtract" },
		{ OPERATOR_TYPE_T::OT_SUBTRACT_FROM, "Subtract from" },
		{ OPERATOR_TYPE_T::OT_MULTIPLY, "Multiply" },
		{ OPERATOR_TYPE_T::OT_MULTIPLY_BY, "Multiply by" },
		{ OPERATOR_TYPE_T::OT_DIVIDE, "Divide" },
		{ OPERATOR_TYPE_T::OT_DIVIDE_BY, " Divide by" },
		{ OPERATOR_TYPE_T::OT_EQUALS , "Equals" },
		{ OPERATOR_TYPE_T::OT_NOT_EQUALS , "Not equals" },
		{ OPERATOR_TYPE_T::OT_GREATER_THAN , "Greater than" },
		{ OPERATOR_TYPE_T::OT_GREATER_THAN_OR_EQUAL , "Greater than or equal" },
		{ OPERATOR_TYPE_T::OT_LESSER_THAN , "Less than" },
		{ OPERATOR_TYPE_T::OT_LESSER_THAN_OR_EQUAL , "Less than or equal" },
		{ OPERATOR_TYPE_T::OT_ASSIGN , "Assign" },
		{ OPERATOR_TYPE_T::OT_NEGATE , "Negate" },
		{ OPERATOR_TYPE_T::OT_INCREMENT , "Increment" },
		{ OPERATOR_TYPE_T::OT_PRE_INCREMENT , "Pre increment" },
		{ OPERATOR_TYPE_T::OT_POST_INCREMENT , "Post increment" },
		{ OPERATOR_TYPE_T::OT_DECREMENT , "Decrement" },
		{ OPERATOR_TYPE_T::OT_PRE_DECREMENT , "Pre decrement" },
		{ OPERATOR_TYPE_T::OT_POST_DECREMENT , "Post decrement" },
		{ OPERATOR_TYPE_T::OT_CONDITIONAL_IF , "Conditional if" },
		{ OPERATOR_TYPE_T::OT_CONDITIONAL_ELSE , "Conditional else" },
		{ OPERATOR_TYPE_T::OT_CONDITIONAL_NULL, "Conditional null" }
	};

	std::string ScriptOperator::translateOperator(OPERATOR_TYPE_T t)
	{
		auto it = opmap.find(t);
		if (it == opmap.end())
			return "";
		return it->second;
	}

	ScriptOperator::ScriptOperator(ScriptScope &scope, OPERATOR_LINK_T linkage)
		: ScriptCallable(scope)
		, linkage(linkage)
	{
	}

	const std::shared_ptr<ScriptType> ScriptOperator::getType() const
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



	std::unique_ptr<ScriptValue> ScriptOperatorMemberAccessor::run(const CALLABLE_PARAMS_T &c)
	{
		// 1. if this->right is scopedCall then provide it with classInstance from this->left
		// 2. this->right must be scoped call - always

		auto leftres = this->left->run();
		auto scv = static_cast<ScriptClassValue*>(leftres->data());

		if (ScriptScopedCall *sc = dynamic_cast<ScriptScopedCall*>(this->right.get()))
		{
			sc->setScope(scv->getValue().get());
		}

		if (ScriptFuncCall *fc = dynamic_cast<ScriptFuncCall*>(this->right.get()))
		{
			fc->setInstance(std::move(leftres));
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



	std::unique_ptr<ScriptValue> ScriptOperatorConditionalNull::run(const CALLABLE_PARAMS_T &c)
	{
		auto v = this->left->run();
		return v->boolean().getValue() ? std::move(v) : this->right->run();
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



	std::unique_ptr<ScriptValue> ScriptOperatorConditionalA::run(const CALLABLE_PARAMS_T &c)
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



	std::unique_ptr<ScriptValue> ScriptOperatorConditionalB::run(const CALLABLE_PARAMS_T &c)
	{
		return this->left->run()->boolean().getValue() ? static_cast<ScriptOperatorConditionalA*>(this->left.get())->right->run() : this->right->run();
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
		if (!dynamic_cast<ScriptOperatorConditionalA*>(this->left.get()))
			throw CompileException("Invalid use of conditional operator");
	}



	std::unique_ptr<ScriptValue> ScriptOperatorNegate::run(const CALLABLE_PARAMS_T &c)
	{
		return nullptr;
	}

	const int ScriptOperatorNegate::getPrecedence() const
	{
		return 3;
	}

	const OPERATOR_TYPE_T ScriptOperatorNegate::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_NEGATE;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorAssign::run(const CALLABLE_PARAMS_T &c)
	{
		if (ScriptOperatorMemberAccessor *memberAccessor = dynamic_cast<ScriptOperatorMemberAccessor*>(this->left.get()))
			memberAccessor->run();

		auto rval = this->right->run();		
		this->varRead->get()->getValue()->assign(*rval);

		return nullptr;
	}

	void ScriptOperatorAssign::setup()
	{
		this->varRead = dynamic_cast<ScriptVarRead*>(this->left.get());

		if (!this->varRead)
		{
			if (ScriptOperatorMemberAccessor *memberAccessor = dynamic_cast<ScriptOperatorMemberAccessor*>(this->left.get()))
				this->varRead = dynamic_cast<ScriptVarRead*>(memberAccessor->right.get());
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



	std::unique_ptr<ScriptValue> ScriptOperatorEquals::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorEquals::getPrecedence() const
	{
		return 10;
	}

	const OPERATOR_TYPE_T ScriptOperatorEquals::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_EQUALS;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorGreaterThan::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorGreaterThan::getPrecedence() const
	{
		return 9;
	}

	const OPERATOR_TYPE_T ScriptOperatorGreaterThan::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_GREATER_THAN;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorGreaterThanOrEqual::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorGreaterThanOrEqual::getPrecedence() const
	{
		return 9;
	}

	const OPERATOR_TYPE_T ScriptOperatorGreaterThanOrEqual::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_GREATER_THAN_OR_EQUAL;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorLessThan::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorLessThan::getPrecedence() const
	{
		return 9;
	}

	const OPERATOR_TYPE_T ScriptOperatorLessThan::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_LESSER_THAN;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorLessThanOrEqual::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorLessThanOrEqual::getPrecedence() const
	{
		return 9;
	}

	const OPERATOR_TYPE_T ScriptOperatorLessThanOrEqual::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_LESSER_THAN_OR_EQUAL;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorAdd::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorAdd::getPrecedence() const
	{
		return 6;
	}

	const OPERATOR_TYPE_T ScriptOperatorAdd::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_ADD;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorAddTo::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorAddTo::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorAddTo::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_ADD_TO;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorSubtract::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorSubtract::getPrecedence() const
	{
		return 6;
	}

	const OPERATOR_TYPE_T ScriptOperatorSubtract::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_SUBTRACT;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorSubtractFrom::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorSubtractFrom::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorSubtractFrom::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_SUBTRACT_FROM;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorMultiply::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorMultiply::getPrecedence() const
	{
		return 5;
	}

	const OPERATOR_TYPE_T ScriptOperatorMultiply::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_MULTIPLY;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorMultiplyBy::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorMultiplyBy::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorMultiplyBy::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_MULTIPLY_BY;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorDivide::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorDivide::getPrecedence() const
	{
		return 5;
	}

	const OPERATOR_TYPE_T ScriptOperatorDivide::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_DIVIDE;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorDivideBy::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorDivideBy::getPrecedence() const
	{
		return 16;
	}

	const OPERATOR_TYPE_T ScriptOperatorDivideBy::getOperatorType() const
	{
		return OPERATOR_TYPE_T::OT_DIVIDE_BY;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorIncrement::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
	}

	const int ScriptOperatorIncrement::getPrecedence() const
	{
		return 2;
	}

	const OPERATOR_TYPE_T ScriptOperatorIncrement::getOperatorType() const
	{
		return this->left ? OPERATOR_TYPE_T::OT_POST_INCREMENT : OPERATOR_TYPE_T::OT_PRE_INCREMENT;
	}



	std::unique_ptr<ScriptValue> ScriptOperatorDecrement::run(const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(this->left.get(), this->right.get());
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