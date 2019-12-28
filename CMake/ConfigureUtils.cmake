option(Utils_USE_DEBUG_ASSERT      "Enable debug asserts" ON)
option(Utils_USE_ITERATOR_DEBUG    "Enable state checks for iterators" OFF)
option(Utils_USE_COMPILER_CHECKS   "Enable compile time asserts." OFF)
option(Utils_NO_DEBUGGER           "Send calls to skPrintf to printf vs skDebugger::report" ON)
option(Utils_BUILD_TESTS           "Build basic tests" OFF)

configure_file(${Utils_SOURCE_DIR}/CMake/skConfig.h.in  ${Utils_SOURCE_DIR}/Config/skConfig.h)
