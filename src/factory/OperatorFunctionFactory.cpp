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
		namespace Functions
		{
			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> Add(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() + static_cast<T_RIGHT*>(right->run()->data())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> AddTo(ScriptCallable *left, ScriptCallable *right)
			{
				auto res = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() + static_cast<T_RIGHT*>(right->run()->data())->getValue());
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> Subtract(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() - static_cast<T_RIGHT*>(right->run()->data())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> SubtractFrom(ScriptCallable *left, ScriptCallable *right)
			{
				auto res = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() - static_cast<T_RIGHT*>(right->run()->data())->getValue());
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> Multiply(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() * static_cast<T_RIGHT*>(right->run()->data())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> MultiplyBy(ScriptCallable *left, ScriptCallable *right)
			{
				auto res = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() * static_cast<T_RIGHT*>(right->run()->data())->getValue());
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> Divide(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() / static_cast<T_RIGHT*>(right->run()->data())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> DivideBy(ScriptCallable *left, ScriptCallable *right)
			{
				auto res = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() / static_cast<T_RIGHT*>(right->run()->data())->getValue());
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> Equal(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() == static_cast<T_RIGHT*>(right->run()->data())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> NotEqual(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() != static_cast<T_RIGHT*>(right->run().get())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> GreaterThan(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() > static_cast<T_RIGHT*>(right->run()->data())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> GreaterOrEqualThan(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() >= static_cast<T_RIGHT*>(right->run()->data())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> LessThan(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() < static_cast<T_RIGHT*>(right->run()->data())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> LessOrEqualThan(ScriptCallable *left, ScriptCallable *right)
			{
				return std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() <= static_cast<T_RIGHT*>(right->run()->data())->getValue());
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> PreDecrement(ScriptCallable *left, ScriptCallable *right)
			{
				auto res = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(right->run()->data())->getValue() - 1);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(right);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> PostDecrement(ScriptCallable *left, ScriptCallable *right)
			{
				auto newVal = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() - 1);
				auto res = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue());
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(newVal->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> PreIncrement(ScriptCallable *left, ScriptCallable *right)
			{
				auto res = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(right->run()->data())->getValue() + 1);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(right);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT, typename T_RESULT>
			std::unique_ptr<ScriptValue> PostIncrement(ScriptCallable *left, ScriptCallable *right)
			{
				auto newVal = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue() + 1);
				auto res = std::make_unique<T_RESULT>(static_cast<T_LEFT*>(left->run()->data())->getValue());
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(newVal->clone());
				return res;
			}
		}

		FUNCMAP_T funcmap = FUNCMAP_T({
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_ADD), &Functions::Add<ScriptIntValue, ScriptIntValue, ScriptIntValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_ADD_TO), &Functions::AddTo<ScriptIntValue, ScriptIntValue, ScriptIntValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_SUBTRACT), &Functions::Subtract<ScriptIntValue, ScriptIntValue, ScriptIntValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_SUBTRACT_FROM), &Functions::SubtractFrom<ScriptIntValue, ScriptIntValue, ScriptIntValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_MULTIPLY), &Functions::Multiply<ScriptIntValue, ScriptIntValue, ScriptIntValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_MULTIPLY_BY), &Functions::MultiplyBy<ScriptIntValue, ScriptIntValue, ScriptIntValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_DIVIDE), &Functions::Divide<ScriptIntValue, ScriptIntValue, ScriptDoubleValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_DIVIDE_BY), &Functions::DivideBy<ScriptIntValue, ScriptIntValue, ScriptDoubleValue> },

			{ std::make_tuple(VALUE_TYPE_T::VT_VOID, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_PRE_DECREMENT), &Functions::PreDecrement<ScriptIntValue, ScriptIntValue, ScriptIntValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_VOID, OPERATOR_TYPE_T::OT_POST_DECREMENT), &Functions::PostDecrement<ScriptIntValue, ScriptIntValue, ScriptIntValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_VOID, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_PRE_INCREMENT), &Functions::PreIncrement<ScriptIntValue, ScriptIntValue, ScriptIntValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_VOID, OPERATOR_TYPE_T::OT_POST_INCREMENT), &Functions::PostIncrement<ScriptIntValue, ScriptIntValue, ScriptIntValue> },

			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_EQUALS), &Functions::Equal<ScriptIntValue, ScriptIntValue, ScriptBoolValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_NOT_EQUALS), &Functions::NotEqual<ScriptIntValue, ScriptIntValue, ScriptBoolValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_GREATER_THAN), &Functions::GreaterThan<ScriptIntValue, ScriptIntValue, ScriptBoolValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_GREATER_THAN_OR_EQUAL), &Functions::GreaterOrEqualThan<ScriptIntValue, ScriptIntValue, ScriptBoolValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_LESSER_THAN), &Functions::LessThan<ScriptIntValue, ScriptIntValue, ScriptBoolValue> },
			{ std::make_tuple(VALUE_TYPE_T::VT_INT, VALUE_TYPE_T::VT_INT, OPERATOR_TYPE_T::OT_LESSER_THAN_OR_EQUAL), &Functions::LessOrEqualThan<ScriptIntValue, ScriptIntValue, ScriptBoolValue> },

			{ std::make_tuple(VALUE_TYPE_T::VT_STRING, VALUE_TYPE_T::VT_STRING, OPERATOR_TYPE_T::OT_ADD), &Functions::Add<ScriptStringValue, ScriptStringValue, ScriptStringValue> },
			});

		OPERATOR_FUNCTION_T OperatorFunctionFactory::getFunction(VALUE_TYPE_T left, VALUE_TYPE_T right, OPERATOR_TYPE_T oper)
		{
			FUNCMAP_T::const_iterator it = funcmap.find(std::make_tuple(left, right, oper));
			
			if (it == funcmap.end())
				throw CompileException(std::string(
					"Could not resolve the operator \"") + ScriptOperator::translateOperator(oper) +
					"\" for types \"" + ScriptType::translateType(left) + "\" and \"" + ScriptType::translateType(right) + "\"");

			return it->second;
		}
	}
}