#ifndef _h_gscript_operator
#define _h_gscript_operator

#include "variable.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "scope.hpp"
#include "factory/OperatorFunctionFactory.hpp"

#include <memory>

namespace gscript
{
	class ParserOperator;
	class ScriptVarRead;

	class ScriptOperator : public ScriptCallable
	{
	public:
		static std::unordered_map<OPERATOR_TYPE_T, std::string> opmap;

		OPERATOR_LINK_T linkage;
		std::shared_ptr<ScriptCallable> left = nullptr;
		std::shared_ptr<ScriptCallable> right = nullptr;

		ScriptOperator(ScriptScope &scope, OPERATOR_LINK_T linkage = OPERATOR_LINK_T::OL_SINGLE);

		virtual const ScriptType *getType() const override;

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override = 0;
		virtual const int getPrecedence() const = 0;
		virtual const OPERATOR_TYPE_T getOperatorType() const = 0;
		virtual bool needFactory() const;
		virtual void setup() {};

		void assignOperatorFunction();

		static std::string translateOperator(OPERATOR_TYPE_T t);

	protected:
		OperatorFunctionFactory::OPERATOR_FUNCTION_T operatorFunction = nullptr;
	};

	class ScriptOperatorMemberAccessor : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
		virtual bool needFactory() const override;
	};

	class ScriptOperatorNegate : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorAssign : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
		virtual bool needFactory() const override;
		virtual void setup() override;

	protected:
		ScriptVarRead *varRead = nullptr;
	};

	class ScriptOperatorEquals : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorGreaterThan : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorLessThan : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorGreaterThanOrEqual : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorLessThanOrEqual : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorAdd : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorAddTo : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorSubtract : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorSubtractFrom : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorMultiply : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorMultiplyBy : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorDivide : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorDivideBy : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorIncrement : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorDecrement : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
	};

	class ScriptOperatorConditionalA : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
		virtual bool needFactory() const override;
	};

	class ScriptOperatorConditionalB : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
		virtual bool needFactory() const override;
		virtual void setup() override;
	};

	class ScriptOperatorConditionalNull : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OPERATOR_TYPE_T getOperatorType() const;
		virtual bool needFactory() const override;
	};
}

#endif