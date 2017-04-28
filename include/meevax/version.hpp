#ifndef INCLUDED_MEEVAX_VERSION_HPP_
#define INCLUDED_MEEVAX_VERSION_HPP_


#include <utilib/string/static_concatenate.hpp>

static constexpr auto project_version {scat("0.1.6")};

static constexpr auto project_major_version {scat("0")};
static constexpr auto project_minor_version {scat("1")};
static constexpr auto project_patch_version {scat("6")};


#endif