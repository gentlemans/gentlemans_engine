#ifndef GE_ASSET_HPP
#define GE_ASSET_HPP

#pragma once

#include <boost/concept_check.hpp>

#include <type_traits>

namespace ge
{
class asset_manager;

namespace concept
{
/// A concpet defining what an asset class looks like.
template <typename X>
struct Asset {
	using loaded_type = typename X::loaded_type;
	using cached = typename X::cached;

	static_assert(
		std::is_same<cached, std::true_type>::value || std::is_same<cached, std::false_type>::value,
		"Cached must be either std::true_type or std::false_type");

	BOOST_CONCEPT_USAGE(Asset) { const char* type = X::asset_type(); }
};
}
}

#endif  // GE_ASSET_HPP
