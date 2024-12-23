#include "Cli.hpp"
#include "Cmd.hpp"
#include "Logger.hpp"
#include "Rustify.hpp"
#include "TermColor.hpp"

#include <cstdlib>
#include <exception>
#include <span>
#include <string>
#include <string_view>
#include <vector>

const Cli&
getCli() noexcept {
  static const Cli cli =  //
      Cli{ "poac" }
          .setDesc("A package manager and build system for C++")
          .addOpt(Opt{ "--verbose" }
                      .setShort("-v")
                      .setDesc("Use verbose output (-vv very verbose output)")
                      .setGlobal(true))
          .addOpt(Opt{ "-vv" }
                      .setDesc("Use very verbose output")
                      .setGlobal(true)
                      .setHidden(true))
          .addOpt(Opt{ "--quiet" }
                      .setShort("-q")
                      .setDesc("Do not print poac log messages")
                      .setGlobal(true))
          .addOpt(Opt{ "--color" }
                      .setDesc("Coloring: auto, always, never")
                      .setPlaceholder("<WHEN>")
                      .setGlobal(true))
          .addOpt(Opt{ "--help" }  //
                      .setShort("-h")
                      .setDesc("Print help")
                      .setGlobal(true))
          .addOpt(Opt{ "--version" }
                      .setShort("-V")
                      .setDesc("Print version info and exit")
                      .setGlobal(false))
          .addOpt(Opt{ "--list" }  //
                      .setDesc("List all subcommands")
                      .setGlobal(false)
                      .setHidden(true))
          .addSubcmd(ADD_CMD)
          .addSubcmd(BUILD_CMD)
          .addSubcmd(CLEAN_CMD)
          .addSubcmd(FMT_CMD)
          .addSubcmd(HELP_CMD)
          .addSubcmd(INIT_CMD)
          .addSubcmd(LINT_CMD)
          .addSubcmd(NEW_CMD)
          .addSubcmd(RUN_CMD)
          .addSubcmd(SEARCH_CMD)
          .addSubcmd(TEST_CMD)
          .addSubcmd(TIDY_CMD)
          .addSubcmd(VERSION_CMD);
  return cli;
}

std::vector<std::string> CLI_ARGS = { "dummy" };  // NOLINT

int
main(int argc, char* argv[]) {
  // Parse arguments (options should appear before the subcommand, as the help
  // message shows intuitively)
  // poac --verbose run --release help --color always --verbose
  // ^^^^^^^^^^^^^^ ^^^^^^^^^^^^^ ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  // [global]       [run]         [help (under run)]
  CLI_ARGS = std::vector<std::string>{ argv, argv + argc };
  const std::span<char* const> args(argv + 1, argv + argc);
  for (auto itr = args.begin(); itr != args.end(); ++itr) {
    if (const auto res = Cli::handleGlobalOpts(itr, args.end())) {
      if (res.value() == Cli::CONTINUE) {
        continue;
      } else {
        return res.value();
      }
    }

    // Local options
    else if (*itr == "-V"sv || *itr == "--version"sv) {
      const std::vector<std::string_view> remArgs(itr + 1, args.end());
      return versionMain(remArgs);
    } else if (*itr == "--list"sv) {
      getCli().printAllSubcmds(true);
      return EXIT_SUCCESS;
    }

    // Subcommands
    else if (getCli().hasSubcmd(*itr)) {
      try {
        const std::vector<std::string_view> remArgs(itr + 1, args.end());
        const int exitCode = getCli().exec(*itr, remArgs);
        if (exitCode != EXIT_SUCCESS) {
          logger::error("'poac {}' failed with exit code `{}`", *itr, exitCode);
        }
        return exitCode;
      } catch (const std::exception& e) {
        logger::error("{}", e.what());
        return EXIT_FAILURE;
      }
    }

    else {
      return getCli().noSuchArg(*itr);
    }
  }

  return getCli().printHelp({});
}
