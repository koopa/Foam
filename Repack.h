#ifndef REPACK_H
#define REPACK_H

#include <tuple>

template<typename TTuple, typename TAddition>
auto tuple_append(const TTuple &tuple, const TAddition &addition)
{
	return std::tuple_cat(tuple, std::make_tuple(addition));
}

template<typename TTuple, typename TAddition>
auto tuple_prepend(const TAddition &addition, const TTuple &tuple)
{
	return std::tuple_cat(std::make_tuple(addition), tuple);
}

template<int TIndex, typename TTuple>
using Element = typename std::tuple_element<TIndex, TTuple>::type;

template<int TIndex, typename TTuple, typename TElement>
struct Repack
{
	static auto value(const TTuple &tuple, const TElement &element)
	{
		return tuple_append(Repack<TIndex - 1, TTuple, TElement>::value(tuple, element), std::get<TIndex>(tuple));
	}
};

template<int TIndex, typename TTuple>
struct Repack<TIndex, TTuple, Element<TIndex, TTuple>>
{
	static auto value(const TTuple &tuple, const Element<TIndex, TTuple> &element)
	{
		return tuple_append(Repack<TIndex - 1, TTuple, Element<TIndex, TTuple>>::value(tuple, element), element);
	}
};

template<typename TTuple, typename TElement>
struct Repack<0, TTuple, TElement>
{
	static auto value(const TTuple &tuple, const TElement &)
	{
		return std::make_tuple(std::get<0>(tuple));
	}
};

template<typename TTuple>
struct Repack<0, TTuple, Element<0, TTuple>>
{
	static auto value(const TTuple &, const Element<0, TTuple> &element)
	{
		return std::make_tuple(element);
	}
};

template<typename TTuple, typename TElement>
TTuple repack(const TTuple &tuple, const TElement &element)
{
	return Repack<std::tuple_size<TTuple>::value - 1, TTuple, TElement>::value(tuple, element);
}

#endif // REPACK_H
