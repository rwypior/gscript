#ifndef _h_gscript_util
#define _h_gscript_util

#include "gscript/lib.hpp"
#include "gscript/defs.hpp"

#include <tuple>
#include <vector>
#include <memory>
#include <cassert>

namespace gscript
{
	class ScriptStatement;

	namespace hash_tuple
	{
		template <typename TT>
		struct hash
		{
			size_t operator()(TT const& tt) const
			{
				return std::hash<TT>()(tt);
			}
		};

		namespace
		{
			template <class T>
			inline void hash_combine(std::size_t& seed, T const& v)
			{
				seed ^= hash_tuple::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}

			template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
			struct HashValueImpl
			{
				static void apply(size_t& seed, Tuple const& tuple)
				{
					HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
					hash_combine(seed, std::get<Index>(tuple));
				}
			};

			template <class Tuple>
			struct HashValueImpl<Tuple, 0>
			{
				static void apply(size_t& seed, Tuple const& tuple)
				{
					hash_combine(seed, std::get<0>(tuple));
				}
			};
		}

		template <typename ... TT>
		struct hash<std::tuple<TT...>>
		{
			size_t operator()(std::tuple<TT...> const& tt) const
			{
				size_t seed = 0;
				HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
				return seed;
			}
		};
	}

	PARAMS_T extractParams(std::vector<std::unique_ptr<ScriptStatement>>& statements);

	template<typename To, typename From>
	std::unique_ptr<To> static_unique_pointer_cast(std::unique_ptr<From>&& from)
	{
		assert(dynamic_cast<To*>(from.get()) && "Incompatible pointers");
		return std::unique_ptr<To>(static_cast<To*>(from.release()));
	}

	template<typename T>
	std::vector<std::unique_ptr<T>> cloneVector(const std::vector<std::unique_ptr<T>>& target)
	{
		std::vector<std::unique_ptr<T>> result(target.size());
		size_t i = 0;
		for (const auto& el : target)
		{
			result[i++] = static_unique_pointer_cast<T>( el->clone() );
		}
		return result;
	}
}

#endif