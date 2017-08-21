#ifndef INCLUDED_MEEVAX_VERSION_HPP_
#define INCLUDED_MEEVAX_VERSION_HPP_


#include <boost/version.hpp>
#include <meevax/string/static_concat.hpp>


namespace meevax {


constexpr auto major_version {meevax::static_concat("0")};
constexpr auto minor_version {meevax::static_concat("2")};
constexpr auto patch_version {meevax::static_concat("4")};

constexpr auto version {meevax::static_concat("0.2.4")};

static constexpr auto boost_major_version {meevax::static_concat("1")};
static constexpr auto boost_minor_version {meevax::static_concat("58")};
static constexpr auto boost_patch_version {meevax::static_concat("0")};

constexpr auto boost_version {meevax::static_concat(
  boost_major_version, ".", boost_minor_version, ".", boost_patch_version
)};

constexpr auto cmake_build_type {meevax::static_concat("Debug")};


} // namespace meevax


#endif
