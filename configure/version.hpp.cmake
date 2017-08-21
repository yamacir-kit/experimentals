#ifndef INCLUDED_MEEVAX_VERSION_HPP_
#define INCLUDED_MEEVAX_VERSION_HPP_


#include <boost/version.hpp>
#include <meevax/string/static_concat.hpp>


namespace ${PROJECT_NAME} {


constexpr auto major_version {meevax::static_concat("${PROJECT_VERSION_MAJOR}")};
constexpr auto minor_version {meevax::static_concat("${PROJECT_VERSION_MINOR}")};
constexpr auto patch_version {meevax::static_concat("${PROJECT_VERSION_PATCH}")};

constexpr auto version {meevax::static_concat("${PROJECT_VERSION}")};

static constexpr auto boost_major_version {meevax::static_concat("${Boost_MAJOR_VERSION}")};
static constexpr auto boost_minor_version {meevax::static_concat("${Boost_MINOR_VERSION}")};
static constexpr auto boost_patch_version {meevax::static_concat("${Boost_SUBMINOR_VERSION}")};

constexpr auto boost_version {meevax::static_concat(
  boost_major_version, ".", boost_minor_version, ".", boost_patch_version
)};

constexpr auto cmake_build_type {meevax::static_concat("${CMAKE_BUILD_TYPE}")};


} // namespace ${PROJECT_NAME}


#endif
