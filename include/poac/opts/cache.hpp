#ifndef POAC_OPTS_CACHE_HPP
#define POAC_OPTS_CACHE_HPP

#include <iostream>
#include <string>
#include <regex>
#include <optional>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include <poac/core/except.hpp>
#include <poac/io/cli.hpp>
#include <poac/io/path.hpp>
#include <poac/io/tar.hpp>
#include <poac/io/yaml.hpp>
#include <poac/util/argparse.hpp>
#include <poac/util/termcolor2.hpp>

namespace poac::opts::cache {
    constexpr auto summary = termcolor2::make_string("Manipulate cache files");
    constexpr auto options = termcolor2::make_string("<command>");

    void clean(const std::vector<std::string>& argv) {
        namespace fs = boost::filesystem;
        if (argv.empty()) {
            // TODO: print help (on 0.6.0)
            std::cout << "Usage: poac cache clean <pkg-name> [-a | --all]" << std::endl;
        }
        else if (util::argparse::use(argv, "-a", "--all")) {
            fs::remove_all(io::path::poac_cache_dir);
        }
        else {
            for (const auto &v : argv) {
                const fs::path pkg = io::path::poac_cache_dir / v;
                if (io::path::validate_dir(pkg)) {
                    fs::remove_all(pkg);
                    std::cout << v << " is deleted" << std::endl;
                } else {
                    std::cout << termcolor2::red<> << v << " not found" << termcolor2::reset<> << std::endl;
                }
            }
        }
    }

    void list(const std::vector<std::string>& argv) {
        namespace fs = boost::filesystem;
        if (argv.empty()) {
            for (const auto &e : boost::make_iterator_range(
                    fs::directory_iterator(io::path::poac_cache_dir), {}))
            {
                std::cout << e.path().filename().string() << std::endl;
            }
        }
        else if (argv.size() == 2 && argv[0] == "--pattern") {
            std::regex pattern(argv[1]);
            for (const auto &e : boost::make_iterator_range(
                    fs::directory_iterator(io::path::poac_cache_dir), {}))
            {
                const std::string cachefile = e.path().filename().string();
                if (std::regex_match(cachefile, pattern))
                    std::cout << cachefile << std::endl;
            }
        }
    }

    void root() {
        std::cout << io::path::poac_cache_dir.string() << std::endl;
    }

    std::optional<core::except::Error>
    check_arguments(const std::vector<std::string>& argv) noexcept {
        namespace except = core::except;
        if (argv.empty()) {
            return except::Error::InvalidSecondArg::Cache{};
        }
        return std::nullopt;
    }

    std::optional<core::except::Error>
    exec(const std::vector<std::string> &argv) {
        namespace except = core::except;

        if (const auto result = check_arguments(argv)) {
            return result;
        } else if (argv[0] == "root" && argv.size() == 1) {
            root();
        } else if (argv[0] == "list") {
            list(std::vector<std::string>(argv.begin() + 1, argv.begin() + argv.size()));
        } else if (argv[0] == "clean") {
            clean(std::vector<std::string>(argv.begin() + 1, argv.begin() + argv.size()));
        } else {
            return except::Error::InvalidSecondArg::Cache{};
        }
        return std::nullopt;
    }
} // end namespace
#endif // !POAC_OPTS_CACHE_HPP
