#pragma once

// external
#include <structopt/app.hpp>

// internal
#include "poac/poac.hpp"

namespace poac::cmd::search {

struct Options : structopt::sub_command {
  /// Package name to search
  String package_name;
};

using NotFound = Error<"No packages found for `{}`", String>;

[[nodiscard]] Fn search(const Options& opts)->Result<void>;

[[nodiscard]] inline Fn exec(const Options& opts)->Result<void> {
  return search(opts);
}

} // namespace poac::cmd::search

STRUCTOPT(poac::cmd::search::Options, package_name);
