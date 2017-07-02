#ifndef INCLUDED_MEEVAX_VERSION_HPP_
#define INCLUDED_MEEVAX_VERSION_HPP_


#include <boost/version.hpp>
#include <meevax/string/static_concatenate.hpp>


static constexpr auto project_major_version {scat("0")};
static constexpr auto project_minor_version {scat("2")};
static constexpr auto project_patch_version {scat("3")};

static constexpr auto project_version {scat("0.2.3")};


static constexpr auto boost_major_version {scat("1")};
static constexpr auto boost_minor_version {scat("58")};
static constexpr auto boost_patch_version {scat("0")};

static constexpr auto boost_version {scat(
  boost_major_version, ".", boost_minor_version, ".", boost_patch_version
)};


static constexpr auto cmake_build_type {scat("Debug")};


#endif
