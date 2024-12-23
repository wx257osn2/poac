# Usage

## Start a new project with Poac

The `poac new` command lets you start a new Poac project:

```console
you:~$ poac new hello_world
     Created binary (application) `hello_world` package
```

> [!TIP]
> If you want to integrate your existing project with Poac, use the `init` command:
>
> ```console
> you:~/your-pj$ poac init
>      Created binary (application) `your-pj` package
> ```
>
> This command just creates a `poac.toml` file not to break your project.

## Build the project

In most cases, you want to execute the generated binary as well as build the project.

```console
you:~/hello_world$ poac run
 Compiling src/main.cc
   Linking hello_world
  Finished debug target(s) in 0.45386s
   Running poac-out/debug/hello_world
Hello, world!
```

If you just want to build it, run the `build` command:

```console
you:~/hello_world$ poac build
  Finished debug target(s) in 0.00866317s
```

Poac uses a cache since we executed the command with no changes.

> [!TIP]
> To use a different compiler, you can export a `CXX` environmental variable:
>
> ```shell
> export CXX=g++-13
> ```

## Install dependencies

Like Cargo does, Poac installs dependencies at build time.  Poac currently supports Git, path, and system dependencies.  You can use two methods to add dependencies to your project: the `poac add` command, and editing `poac.toml` directly.

### `poac add`

The `poac add` command accepts the following arguments:

`poac add <package names ....> --<options>`

Options:
- `--sys`: Marks the packages as system dependency (requires the `--version` argument)
- `--version`: Specify dependency version. Only used with system dependencies
- `--rev`: Specify revision for git dependencies
- `--tag`: Specify tag for git dependencies
- `--branch`: Specify branch for git dependencies

Example
```bash
poac add libgit2 --sys --version "1.1.0"
poac add "ToruNiina/toml11" --rev "846abd9a49082fe51440aa07005c360f13a67bbf"
```

If `tag`, `branch`, or `rev` is unspecified for git dependencies, Poac will use the latest revision of the default branch. System dependency names must be acceptable by `pkg-config`. The version requirement syntax is specified in [src/VersionReq.hpp](https://github.com/poac-dev/poac/blob/main/src/VersionReq.hpp).

### editing `poac.toml` directly

The syntax for `poac.toml` like below:

```toml
[dependencies]

# git dependency
"ToruNiina/toml11" = { git = "https://github.com/ToruNiina/toml11.git", rev = "846abd9a49082fe51440aa07005c360f13a67bbf" }

# path dependency
local_lib = { path = "../local_lib" }

# system dependency
fmt = { version = ">= 9", system = true }
```

If `tag`, `branch`, or `rev` is unspecified for git dependencies, Poac will use the latest revision of the default branch. System dependency names must be acceptable by `pkg-config`. The version requirement syntax is specified in [src/VersionReq.hpp](https://github.com/poac-dev/poac/blob/main/src/VersionReq.hpp).


After adding dependencies, executing the `build` command will install the package and its dependencies.

```console
you:~/hello_world$ poac build
Downloaded ToruNiina/toml11 846abd9a49082fe51440aa07005c360f13a67bbf
 Compiling src/main.cc
   Linking hello_world
  Finished debug target(s) in 0.70s
```

> [!WARNING]
> Poac currently supports building a project with header-only dependencies.
> Building with build-required dependencies will be soon supported.

## Unit tests

You can write unit tests in any source files within the `src` directory.  Create a new file like:

`src/Lib.cc`

```cpp
int add(int a, int b) {
  return a + b;
}

#ifdef POAC_TEST

#  include <cassert>

int main() {
  assert(add(1, 2) == 3);  // ok
  assert(add(1, 2) == 4);  // fail
}

#endif
```

Now, with the `test` command, you can run tests defined within `POAC_TEST`:

```console
you:~/hello_world$ poac test
 Compiling src/Lib.cc
   Linking tests/test_Lib
   Testing Lib
Assertion failed: (add(1, 2) == 4), function main, file Lib.cc, line 13.
make: *** [test] Abort trap: 6
```

Unit tests with the `POAC_TEST` macro are useful when testing private functions.  Integration testing with the `tests` directory has not yet been implemented.

## Run linter

Linting source code is essential to protect its quality.  Poac supports linting your project by the `lint` command:

```console
you:~/hello_world$ poac lint
   Linting hello_world
src/main.cc:0:  No copyright message found.  You should have a line: "Copyright [year] <Copyright Owner>"  [legal/copyright] [5]
Done processing src/main.cc
Total errors found: 1
Error: `cpplint` exited with status 1
```

> [!TIP]
> If you do not have `cpplint`, install it with the following command:
>
> ```bash
> pip install cpplint
> ```

The `lint` command works without configurations, and Poac would automatically opt out of unwanted lints by adjusting to each project.
To customize the lint settings, try adding the `[lint.cpplint]` key in your `poac.toml` like [this](https://github.com/poac-dev/poac/blob/cc30b706fb49860903384df56d650a0955aca16c/poac.toml#L67-L83)
or creating a [`CPPLINT.cfg`](https://github.com/poac-dev/poac/blob/5e7e3792e8818d165149214e94f30958fb0fef66/CPPLINT.cfg) file in the repository root.

## Run formatter

Poac also supports formatting your source code with `clang-format`.  Ensure having installed `clang-format` before running this command.

```console
you:~/hello_world$ poac fmt
  Formatting hello_world
```

> [!NOTE]
> This command automatically detects what files we need to format to avoid getting bothered by commands like:
>
> ```console
> $ # We need to avoid the `build` dir and such dirs ...
> $ clang-format ./src/*.cpp -i
> $ clang-format ./include/**/*.hpp -i
> $ clang-format ./tests/**/*.cpp -i
> $ ...
> ```

To customize the format settings, try creating a [`.clang-format`](https://github.com/poac-dev/poac/blob/main/.clang-format) file to the repository root.

## Run `clang-tidy`

Poac also supports running `clang-tidy` on your source code.  Ensure having installed `clang-tidy` before running this command.

```console
you:~/hello_world$ poac tidy
  Running clang-tidy
```

You can customize the tidy settings by creating a [`.clang-tidy`](https://github.com/poac-dev/poac/blob/main/.clang-tidy) file to the repository root.
