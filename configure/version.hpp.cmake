#ifndef INCLUDED_MEEVAX_VERSION_HPP_
#define INCLUDED_MEEVAX_VERSION_HPP_


#include <meevax/string/static_concatenate.hpp>

static constexpr auto project_version {scat("${PROJECT_VERSION}")};

static constexpr auto project_major_version {scat("${PROJECT_VERSION_MAJOR}")};
static constexpr auto project_minor_version {scat("${PROJECT_VERSION_MINOR}")};
static constexpr auto project_patch_version {scat("${PROJECT_VERSION_PATCH}")};

static constexpr auto cmake_build_type {scat("${CMAKE_BUILD_TYPE}")};


#endif
