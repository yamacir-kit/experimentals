#ifndef INCLUDED_MEEVAX_CONFIGURE_VERSION_HPP
#define INCLUDED_MEEVAX_CONFIGURE_VERSION_HPP


#include <boost/version.hpp>


#if __cplusplus != 201703L
#include <string>
#else
#include <meevax/string/static_concat.hpp>
#endif


namespace ${PROJECT_NAME} {


#if __cplusplus != 201703L
const std::string project_name {"${PROJECT_NAME}"};

const std::string build_time {"${${PROJECT_NAME}_TIMESTAMP}"};
const std::string build_type {"${CMAKE_BUILD_TYPE}"};

const std::string major_version {"${PROJECT_VERSION_MAJOR}"};
const std::string minor_version {"${PROJECT_VERSION_MINOR}"};
const std::string patch_version {"${PROJECT_VERSION_PATCH}"};

const std::string version {"${PROJECT_VERSION}"};

const std::string boost_major_version {"${Boost_MAJOR_VERSION}"};
const std::string boost_minor_version {"${Boost_MINOR_VERSION}"};
const std::string boost_patch_version {"${Boost_SUBMINOR_VERSION}"};

const std::string boost_version {
  boost_major_version + std::string {"."} + boost_minor_version + std::string {"."} + boost_patch_version
};
#else
constexpr auto project_name {meevax::static_concat("${PROJECT_NAME}")};

constexpr auto build_time {meevax::static_concat("${${PROJECT_NAME}_TIMESTAMP}")};
constexpr auto build_time {meevax::static_concat("${CMAKE_BUILD_TYPE}")};

constexpr auto major_version {meevax::static_concat("${PROJECT_VERSION_MAJOR}")};
constexpr auto minor_version {meevax::static_concat("${PROJECT_VERSION_MINOR}")};
constexpr auto patch_version {meevax::static_concat("${PROJECT_VERSION_PATCH}")};

constexpr auto version {meevax::static_concat("${PROJECT_VERSION}")};

constexpr auto boost_major_version {meevax::static_concat("${Boost_MAJOR_VERSION}")};
constexpr auto boost_minor_version {meevax::static_concat("${Boost_MINOR_VERSION}")};
constexpr auto boost_patch_version {meevax::static_concat("${Boost_SUBMINOR_VERSION}")};

constexpr auto boost_version {meevax::static_concat(
  boost_major_version, ".", boost_minor_version, ".", boost_patch_version
)};
#endif


} // namespace ${PROJECT_NAME}


#endif

