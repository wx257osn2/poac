include_guard(GLOBAL)

message(CHECK_START "Adding Poac dependencies")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
unset(missingDependencies)

if (DEFINED POAC_DEPS_DIR)
    if (NOT DEFINED BOOST_ROOT)
        set(BOOST_ROOT "${POAC_DEPS_DIR}/boost")
    endif ()
    if (NOT DEFINED LIBGIT2_DIR)
        set(LIBGIT2_DIR "${POAC_DEPS_DIR}/libgit2")
    endif ()
endif ()

include(FetchContent)
set(DEPENDENCIES Boost Clipp Fmt LibArchive LibGit2 MitamaCppResult OpenSSL Plog Toml11)
foreach (DEP IN LISTS DEPENDENCIES)
    include(cmake/Add${DEP}.cmake)
endforeach()

list(POP_BACK CMAKE_MESSAGE_INDENT)
if(missingDependencies)
    message(CHECK_FAIL "missing dependencies: ${missingDependencies}")
    message(FATAL_ERROR "missing dependencies found")
else()
    message(CHECK_PASS "all dependencies are added")
endif()

message(STATUS "dependencies are ... ${POAC_DEPENDENCIES}")