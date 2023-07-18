#pragma once
#include <type_traits>

template<typename enum_t>
struct enable_bit_mask_operators_t
{
	static constexpr bool enable = false;
};

template<typename enum_t>
std::enable_if_t<enable_bit_mask_operators_t<enum_t>::enable, enum_t>
operator |(enum_t lhs, enum_t rhs)
{
	using underlying = std::underlying_type_t<enum_t>;

	return static_cast<enum_t> (
		static_cast<underlying>(lhs) |
		static_cast<underlying>(rhs)
		);
}

template<typename enum_t>
std::enable_if_t<enable_bit_mask_operators_t<enum_t>::enable, enum_t>
operator &(enum_t lhs, enum_t rhs)
{
	using underlying = std::underlying_type_t<enum_t>;
	return static_cast<enum_t> (
		static_cast<underlying>(lhs) &
		static_cast<underlying>(rhs)
		);
}

template<typename enum_t>
std::enable_if_t<enable_bit_mask_operators_t<enum_t>::enable, enum_t>
operator &=(enum_t& lhs, enum_t rhs)
{
	using underlying = std::underlying_type_t<enum_t>;
	lhs = static_cast<enum_t> (
		static_cast<underlying>(lhs) &
		static_cast<underlying>(rhs)
		);

	return lhs;
}

template<typename enum_t>
std::enable_if_t<enable_bit_mask_operators_t<enum_t>::enable, enum_t>
operator |=(enum_t& lhs, enum_t rhs)
{
	using underlying = std::underlying_type_t<enum_t>;
	lhs = static_cast<enum_t> (
		static_cast<underlying>(lhs) |
		static_cast<underlying>(rhs)
		);

	return lhs;
}

template<typename enum_t>
std::enable_if_t<enable_bit_mask_operators_t<enum_t>::enable, enum_t>
operator ~(enum_t lhs)
{
	using underlying = std::underlying_type_t<enum_t>;
	lhs = static_cast<enum_t> (
		~static_cast<underlying>(lhs));

	return lhs;
}

template<typename enum_t>
std::enable_if_t<enable_bit_mask_operators_t<enum_t>::enable, bool> IsSet(enum_t lhs, enum_t rhs)
{
	return (lhs & rhs) == rhs;
}


#define ENABLE_BITMASK_OPERATORS(x)  \
template<>                           \
struct EnableBitMaskOperators<x>     \
{                                    \
    static const bool enable = true; \
};									 
