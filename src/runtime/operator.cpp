#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/scopedCall.hpp"
#include "gscript/runtime/funcCall.hpp"
#include "gscript/compileException.hpp"
#include "gscript/debug.hpp"

#include <string>
#include <cassert>

namespace gscript
{
	std::unordered_map<OperatorType, std::string> ScriptOperator::opmap = {
		{ OperatorType::Invalid, "Invalid" },
		{ OperatorType::MemberAccessor, "Member accessor" },
		{ OperatorType::Add, "Add" },
		{ OperatorType::AddTo, "Add to" },
		{ OperatorType::Subtract, "Subtract" },
		{ OperatorType::SubtractFrom, "Subtract from" },
		{ OperatorType::Multiply, "Multiply" },
		{ OperatorType::MultiplyBy, "Multiply by" },
		{ OperatorType::Divide, "Divide" },
		{ OperatorType::DivideBy, " Divide by" },
		{ OperatorType::Equals , "Equals" },
		{ OperatorType::NotEquals, "Not equals" },
		{ OperatorType::GreaterThan, "Greater than" },
		{ OperatorType::GreaterThanOrEqual, "Greater than or equal" },
		{ OperatorType::LesserThan, "Less than" },
		{ OperatorType::LesserThanOrEqual, "Less than or equal" },
		{ OperatorType::Assign, "Assign" },
		{ OperatorType::Negate, "Negate" },
		{ OperatorType::Increment, "Increment" },
		{ OperatorType::PreIncrement, "Pre increment" },
		{ OperatorType::PostIncrement, "Post increment" },
		{ OperatorType::Decrement, "Decrement" },
		{ OperatorType::PreDecrement, "Pre decrement" },
		{ OperatorType::PostDecrement, "Post decrement" },
		{ OperatorType::ConditionalIf, "Conditional if" },
		{ OperatorType::ConditionalElse, "Conditional else" },
		{ OperatorType::ConditionalNull, "Conditional null" }
	};

	std::string ScriptOperator::translateOperator(OperatorType t)
	{
		auto it = opmap.find(t);
		if (it == opmap.end())
			return "";
		return it->second;
	}

	ScriptOperator::ScriptOperator(const ScriptOperator& b)
		: linkage(b.linkage)
		, left(b.left->clone())
		, right(b.right->clone())
		, operatorFunction(b.operatorFunction)
	{
	}

	ScriptOperator::ScriptOperator(OperatorLinkage linkage)
		: linkage(linkage)
	{
	}

	const std::shared_ptr<ScriptType> ScriptOperator::getType() const
	{
		return this->right ? this->right->getType() : this->left->getType();
	}

	void ScriptOperator::assignOperatorFunction()
	{
		ValueType typeLeft = this->left ? this->left->getType()->getTypeDescriptor() : ValueType::Void;
		ValueType typeRight = this->right ? this->right->getType()->getTypeDescriptor() : ValueType::Void;

		this->operatorFunction = OperatorFunctionFactory::getFunction(typeLeft, typeRight, this->getOperatorType());
	}

	bool ScriptOperator::needFactory() const
	{
		return true;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorMemberAccessor::clone()
	{
		return std::make_unique<ScriptOperatorMemberAccessor>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorMemberAccessor::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		// 1. if this->right is scopedCall then provide it with classInstance from this->left
		// 2. this->right must be scoped call - always

		auto leftres = this->left->run(scope);
		auto scv = static_cast<ScriptClassValue*>(leftres->data());

		if (ScriptScopedCall *sc = dynamic_cast<ScriptScopedCall*>(this->right.get()))
		{
			sc->setScope(scv->getValue().get());
		}

		if (ScriptFuncCall *fc = dynamic_cast<ScriptFuncCall*>(this->right.get()))
		{
			fc->setInstance(std::move(leftres));
		}

		return this->right->run(scope, c);
	}

	const int ScriptOperatorMemberAccessor::getPrecedence() const
	{
		return 2;
	}

	const OperatorType ScriptOperatorMemberAccessor::getOperatorType() const
	{
		return OperatorType::MemberAccessor;
	}

	bool ScriptOperatorMemberAccessor::needFactory() const
	{
		return false;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorConditionalNull::clone()
	{
		return std::make_unique<ScriptOperatorConditionalNull>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorConditionalNull::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		auto v = this->left->run(scope);
		return v->boolean().getValue() ? std::move(v) : this->right->run(scope);
	}

	const int ScriptOperatorConditionalNull::getPrecedence() const
	{
		return 16;
	}

	const OperatorType ScriptOperatorConditionalNull::getOperatorType() const
	{
		return OperatorType::ConditionalNull;
	}

	bool ScriptOperatorConditionalNull::needFactory() const
	{
		return false;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorConditionalA::clone()
	{
		return std::make_unique<ScriptOperatorConditionalA>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorConditionalA::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->left->run(scope);
	}

	const int ScriptOperatorConditionalA::getPrecedence() const
	{
		return 16;
	}

	const OperatorType ScriptOperatorConditionalA::getOperatorType() const
	{
		return OperatorType::ConditionalIf;
	}

	bool ScriptOperatorConditionalA::needFactory() const
	{
		return false;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorConditionalB::clone()
	{
		return std::make_unique<ScriptOperatorConditionalB>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorConditionalB::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->left->run(scope)->boolean().getValue() ? static_cast<ScriptOperatorConditionalA*>(this->left.get())->right->run(scope) : this->right->run(scope);
	}

	const int ScriptOperatorConditionalB::getPrecedence() const
	{
		return 16;
	}

	const OperatorType ScriptOperatorConditionalB::getOperatorType() const
	{
		return OperatorType::ConditionalElse;
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



	std::unique_ptr<ScriptCallable> ScriptOperatorNegate::clone()
	{
		return std::make_unique<ScriptOperatorNegate>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorNegate::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return nullptr;
	}

	const int ScriptOperatorNegate::getPrecedence() const
	{
		return 3;
	}

	const OperatorType ScriptOperatorNegate::getOperatorType() const
	{
		return OperatorType::Negate;
	}



	ScriptOperatorAssign::ScriptOperatorAssign(const ScriptOperatorAssign& b)
		: ScriptOperator(b)
	{
		this->setup();
	}

	std::unique_ptr<ScriptCallable> ScriptOperatorAssign::clone()
	{
		return std::make_unique<ScriptOperatorAssign>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorAssign::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		if (ScriptOperatorMemberAccessor *memberAccessor = dynamic_cast<ScriptOperatorMemberAccessor*>(this->left.get()))
			memberAccessor->run(scope);

		auto rval = this->right->run(scope);
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

	const OperatorType ScriptOperatorAssign::getOperatorType() const
	{
		return OperatorType::Assign;
	}

	bool ScriptOperatorAssign::needFactory() const
	{
		return false;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorEquals::clone()
	{
		return std::make_unique<ScriptOperatorEquals>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorEquals::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorEquals::getPrecedence() const
	{
		return 10;
	}

	const OperatorType ScriptOperatorEquals::getOperatorType() const
	{
		return OperatorType::Equals;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorGreaterThan::clone()
	{
		return std::make_unique<ScriptOperatorGreaterThan>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorGreaterThan::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorGreaterThan::getPrecedence() const
	{
		return 9;
	}

	const OperatorType ScriptOperatorGreaterThan::getOperatorType() const
	{
		return OperatorType::GreaterThan;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorGreaterThanOrEqual::clone()
	{
		return std::make_unique<ScriptOperatorGreaterThanOrEqual>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorGreaterThanOrEqual::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorGreaterThanOrEqual::getPrecedence() const
	{
		return 9;
	}

	const OperatorType ScriptOperatorGreaterThanOrEqual::getOperatorType() const
	{
		return OperatorType::GreaterThanOrEqual;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorLessThan::clone()
	{
		return std::make_unique<ScriptOperatorLessThan>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorLessThan::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorLessThan::getPrecedence() const
	{
		return 9;
	}

	const OperatorType ScriptOperatorLessThan::getOperatorType() const
	{
		return OperatorType::LesserThan;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorLessThanOrEqual::clone()
	{
		return std::make_unique<ScriptOperatorLessThanOrEqual>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorLessThanOrEqual::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorLessThanOrEqual::getPrecedence() const
	{
		return 9;
	}

	const OperatorType ScriptOperatorLessThanOrEqual::getOperatorType() const
	{
		return OperatorType::LesserThanOrEqual;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorAdd::clone()
	{
		return std::make_unique<ScriptOperatorAdd>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorAdd::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorAdd::getPrecedence() const
	{
		return 6;
	}

	const OperatorType ScriptOperatorAdd::getOperatorType() const
	{
		return OperatorType::Add;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorAddTo::clone()
	{
		return std::make_unique<ScriptOperatorAddTo>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorAddTo::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorAddTo::getPrecedence() const
	{
		return 16;
	}

	const OperatorType ScriptOperatorAddTo::getOperatorType() const
	{
		return OperatorType::AddTo;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorSubtract::clone()
	{
		return std::make_unique<ScriptOperatorSubtract>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorSubtract::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorSubtract::getPrecedence() const
	{
		return 6;
	}

	const OperatorType ScriptOperatorSubtract::getOperatorType() const
	{
		return OperatorType::Subtract;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorSubtractFrom::clone()
	{
		return std::make_unique<ScriptOperatorSubtractFrom>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorSubtractFrom::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorSubtractFrom::getPrecedence() const
	{
		return 16;
	}

	const OperatorType ScriptOperatorSubtractFrom::getOperatorType() const
	{
		return OperatorType::SubtractFrom;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorMultiply::clone()
	{
		return std::make_unique<ScriptOperatorMultiply>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorMultiply::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorMultiply::getPrecedence() const
	{
		return 5;
	}

	const OperatorType ScriptOperatorMultiply::getOperatorType() const
	{
		return OperatorType::Multiply;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorMultiplyBy::clone()
	{
		return std::make_unique<ScriptOperatorMultiplyBy>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorMultiplyBy::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorMultiplyBy::getPrecedence() const
	{
		return 16;
	}

	const OperatorType ScriptOperatorMultiplyBy::getOperatorType() const
	{
		return OperatorType::MultiplyBy;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorDivide::clone()
	{
		return std::make_unique<ScriptOperatorDivide>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorDivide::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorDivide::getPrecedence() const
	{
		return 5;
	}

	const OperatorType ScriptOperatorDivide::getOperatorType() const
	{
		return OperatorType::Divide;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorDivideBy::clone()
	{
		return std::make_unique<ScriptOperatorDivideBy>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorDivideBy::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorDivideBy::getPrecedence() const
	{
		return 16;
	}

	const OperatorType ScriptOperatorDivideBy::getOperatorType() const
	{
		return OperatorType::DivideBy;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorIncrement::clone()
	{
		return std::make_unique<ScriptOperatorIncrement>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorIncrement::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorIncrement::getPrecedence() const
	{
		return 2;
	}

	const OperatorType ScriptOperatorIncrement::getOperatorType() const
	{
		return this->left ? OperatorType::PostIncrement : OperatorType::PreIncrement;
	}



	std::unique_ptr<ScriptCallable> ScriptOperatorDecrement::clone()
	{
		return std::make_unique<ScriptOperatorDecrement>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptOperatorDecrement::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->operatorFunction(scope, this->left.get(), this->right.get());
	}

	const int ScriptOperatorDecrement::getPrecedence() const
	{
		return 2;
	}

	const OperatorType ScriptOperatorDecrement::getOperatorType() const
	{
		return this->left ? OperatorType::PostDecrement : OperatorType::PreDecrement;
	}
}