#ifndef INCLUDED_MEEVAX_CONFIGURE_VERSION_HPP
#define INCLUDED_MEEVAX_CONFIGURE_VERSION_HPP


#include <boost/version.hpp>


#if __cplusplus < 201402L
#include <string>
#else
#include <meevax/string/static_concat.hpp>
#endif


namespace meevax {


#if __cplusplus < 201402L
const std::string project_name {"meevax"};

const std::string build_time {"2017/11/11 00:12:45"};
const std::string build_type {"Debug"};

const std::string major_version {"0"};
const std::string minor_version {"3"};
const std::string patch_version {"0"};

const std::string version {"0.3.0"};

const std::string boost_major_version {"1"};
const std::string boost_minor_version {"58"};
const std::string boost_patch_version {"0"};

const std::string boost_version {
  boost_major_version + std::string {"."} + boost_minor_version + std::string {"."} + boost_patch_version
};
#else
constexpr auto project_name {meevax::static_concat("meevax")};

constexpr auto build_time {meevax::static_concat("2017/11/11 00:12:45")};
constexpr auto build_type {meevax::static_concat("Debug")};

constexpr auto major_version {meevax::static_concat("0")};
constexpr auto minor_version {meevax::static_concat("3")};
constexpr auto patch_version {meevax::static_concat("0")};

constexpr auto version {meevax::static_concat("0.3.0")};

constexpr auto boost_major_version {meevax::static_concat("1")};
constexpr auto boost_minor_version {meevax::static_concat("58")};
constexpr auto boost_patch_version {meevax::static_concat("0")};

constexpr auto boost_version {meevax::static_concat(
  boost_major_version, ".", boost_minor_version, ".", boost_patch_version
)};
#endif


} // namespace meevax


#endif
