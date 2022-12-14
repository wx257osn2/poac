#pragma once

// std
#include <string>

// internal
#include "poac/core/builder/data.hpp"
#include "poac/poac.hpp"

namespace poac::core::builder::log {

using FailedToLoadBuildLog =
    Error<"loading build log `{}`: {}", String, String>;
using FailedToOpenBuildLog = Error<"opening build log: {}", String>;
using FailedToLoadDepsLog = Error<"loading deps log `{}`: {}", String, String>;
using FailedToOpenDepsLog = Error<"opening deps log: {}", String>;

inline constexpr StringRef build_log_file_name = ".ninja_log";
inline constexpr StringRef deps_log_file_name = ".ninja_deps";

[[nodiscard]] Fn load_build_log(data::NinjaMain& ninja_main)->Result<void>;

[[nodiscard]] Fn load_deps_log(data::NinjaMain& ninja_main)->Result<void>;

} // namespace poac::core::builder::log
