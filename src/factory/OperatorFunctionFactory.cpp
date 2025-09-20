#include "factory/OperatorFunctionFactory.hpp"
#include "runtime/scriptValue.hpp"
#include "runtime/callable.hpp"
#include "runtime/varRead.hpp"
#include "runtime/variable.hpp"
#include "runtime/operator.hpp"
#include "type.hpp"
#include "compileException.hpp"

namespace gscript
{
	namespace OperatorFunctionFactory
	{
		// Type converters

		template<typename>
		struct OperatorReturnType;

		template<>
		struct OperatorReturnType<bool>
		{
			using type = ScriptBoolValue;
		};

		template<>
		struct OperatorReturnType<char>
		{
			using type = ScriptCharValue;
		};

		template<>
		struct OperatorReturnType<unsigned char>
		{
			using type = ScriptByteValue;
		};

		template<>
		struct OperatorReturnType<int>
		{
			using type = ScriptIntValue;
		};

		template<>
		struct OperatorReturnType<unsigned int>
		{
			using type = ScriptUnsignedIntValue;
		};

		template<>
		struct OperatorReturnType<float>
		{
			using type = ScriptFloatValue;
		};

		template<>
		struct OperatorReturnType<double>
		{
			using type = ScriptDoubleValue;
		};

		template<>
		struct OperatorReturnType<std::string>
		{
			using type = ScriptStringValue;
		};
		
		template<>
		struct OperatorReturnType<ScriptNull>
		{
			using type = ScriptNullValue;
		};

		// Functions

		namespace Functions
		{
#pragma warning(push)
#pragma warning(disable: 4804 4805)

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Add(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() + static_cast<T_RIGHT*>(right->run()->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> AddTo(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() + static_cast<T_RIGHT*>(right->run()->data())->getValue();
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Subtract(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() - static_cast<T_RIGHT*>(right->run()->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> SubtractFrom(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() - static_cast<T_RIGHT*>(right->run()->data())->getValue();
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Multiply(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() * static_cast<T_RIGHT*>(right->run()->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> MultiplyBy(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() * static_cast<T_RIGHT*>(right->run()->data())->getValue();
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Divide(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() / static_cast<T_RIGHT*>(right->run()->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> DivideBy(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() / static_cast<T_RIGHT*>(right->run()->data())->getValue();
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Equal(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() == static_cast<T_RIGHT*>(right->run()->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> NotEqual(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() != static_cast<T_RIGHT*>(right->run().get())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> GreaterThan(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() > static_cast<T_RIGHT*>(right->run()->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> GreaterOrEqualThan(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() >= static_cast<T_RIGHT*>(right->run()->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> LessThan(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() < static_cast<T_RIGHT*>(right->run()->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> LessOrEqualThan(ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run()->data())->getValue() <= static_cast<T_RIGHT*>(right->run()->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_RIGHT>
			std::unique_ptr<ScriptValue> PreDecrement(ScriptCallable*, ScriptCallable *right)
			{
				auto val = static_cast<T_RIGHT*>(right->run()->data())->getValue() - 1;
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(right);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT>
			std::unique_ptr<ScriptValue> PostDecrement(ScriptCallable *left, ScriptCallable*)
			{
				auto data = left->run()->data();
				auto val = static_cast<T_LEFT*>(data)->getValue() - 1;
				auto newVal = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				auto res = data->clone();
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(std::move(newVal));
				return res;
			}

			template<typename T_RIGHT>
			std::unique_ptr<ScriptValue> PreIncrement(ScriptCallable*, ScriptCallable *right)
			{
				auto val = static_cast<T_RIGHT*>(right->run()->data())->getValue() + 1;
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(right);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT>
			std::unique_ptr<ScriptValue> PostIncrement(ScriptCallable *left, ScriptCallable*)
			{
				auto data = left->run()->data();
				auto val = static_cast<T_LEFT*>(data)->getValue() + 1;
				auto newVal = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				auto res = data->clone();
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(std::move(newVal));
				return res;
			}

#pragma warning(pop)
		}

		// Factory

		template<typename L, typename R>
		OPERATOR_FUNCTION_T getFunction(OPERATOR_TYPE_T func)
		{
			switch (func)
			{
			case OPERATOR_TYPE_T::OT_ADD: return &Functions::Add<L, R>;
			case OPERATOR_TYPE_T::OT_ADD_TO: return &Functions::AddTo<L, R>;
			case OPERATOR_TYPE_T::OT_SUBTRACT: return &Functions::Subtract<L, R>;
			case OPERATOR_TYPE_T::OT_SUBTRACT_FROM: return &Functions::SubtractFrom<L, R>;
			case OPERATOR_TYPE_T::OT_MULTIPLY: return &Functions::Multiply<L, R>;
			case OPERATOR_TYPE_T::OT_MULTIPLY_BY: return &Functions::MultiplyBy<L, R>;
			case OPERATOR_TYPE_T::OT_DIVIDE: return &Functions::Divide<L, R>;
			case OPERATOR_TYPE_T::OT_DIVIDE_BY: return &Functions::DivideBy<L, R>;

			case OPERATOR_TYPE_T::OT_PRE_DECREMENT: return &Functions::PreDecrement<R>;
			case OPERATOR_TYPE_T::OT_POST_DECREMENT: return &Functions::PostDecrement<L>;
			case OPERATOR_TYPE_T::OT_PRE_INCREMENT: return &Functions::PreIncrement<R>;
			case OPERATOR_TYPE_T::OT_POST_INCREMENT: return &Functions::PostIncrement<L>;

			case OPERATOR_TYPE_T::OT_EQUALS: return &Functions::Equal<L, R>;
			case OPERATOR_TYPE_T::OT_GREATER_THAN: return &Functions::GreaterThan<L, R>;
			case OPERATOR_TYPE_T::OT_GREATER_THAN_OR_EQUAL: return &Functions::GreaterOrEqualThan<L, R>;
			case OPERATOR_TYPE_T::OT_LESSER_THAN: return &Functions::LessThan<L, R>;
			case OPERATOR_TYPE_T::OT_LESSER_THAN_OR_EQUAL: return &Functions::LessOrEqualThan<L, R>;
			}

			return nullptr;
		}

		template<typename T>
		OPERATOR_FUNCTION_T getFunction(VALUE_TYPE_T right, OPERATOR_TYPE_T func)
		{
			switch (right)
			{
			case VALUE_TYPE_T::VT_BOOL: return getFunction<T, ScriptBoolValue>(func);
			case VALUE_TYPE_T::VT_CHAR: return getFunction<T, ScriptCharValue>(func);
			case VALUE_TYPE_T::VT_BYTE: return getFunction<T, ScriptByteValue>(func);
			case VALUE_TYPE_T::VT_INT: return getFunction <T, ScriptIntValue> (func);
			case VALUE_TYPE_T::VT_UNSIGNED_INT: return getFunction<T, ScriptUnsignedIntValue>(func);
			case VALUE_TYPE_T::VT_FLOAT: return getFunction<T, ScriptFloatValue>(func);
			case VALUE_TYPE_T::VT_DOUBLE: return getFunction<T, ScriptDoubleValue>(func);
			case VALUE_TYPE_T::VT_VOID: return getFunction<T, ScriptNullValue>(func);
			}

			return nullptr;
		}

		OPERATOR_FUNCTION_T getFunction(VALUE_TYPE_T left, VALUE_TYPE_T right, OPERATOR_TYPE_T func)
		{
			switch (left)
			{
			case VALUE_TYPE_T::VT_BOOL: return getFunction<ScriptBoolValue>(right, func);
			case VALUE_TYPE_T::VT_CHAR: return getFunction<ScriptCharValue>(right, func);
			case VALUE_TYPE_T::VT_BYTE: return getFunction<ScriptByteValue>(right, func);
			case VALUE_TYPE_T::VT_INT: return getFunction<ScriptIntValue>(right, func);
			case VALUE_TYPE_T::VT_UNSIGNED_INT: return getFunction<ScriptUnsignedIntValue>(right, func);
			case VALUE_TYPE_T::VT_FLOAT: return getFunction<ScriptFloatValue>(right, func);
			case VALUE_TYPE_T::VT_DOUBLE: return getFunction<ScriptDoubleValue>(right, func);
			case VALUE_TYPE_T::VT_VOID: return getFunction<ScriptNullValue>(right, func);
			}

			return nullptr;
		}
	}
}