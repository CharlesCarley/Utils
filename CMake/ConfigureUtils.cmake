set(Utils_USE_MEMORY_ALLOCATOR  "0"             CACHE STRING    "Allocation back end (0 = standard, 1 = dl)")
set(Utils_USE_SIGNED_SIZE_T     FALSE           CACHE BOOL      "Define SKsize to int")
set(Utils_USE_STD_STRING_FUNCS  TRUE            CACHE BOOL      "Use system strlen, strcpy ...")
set(Utils_USE_DEBUG_ASSERT      TRUE            CACHE BOOL      "Enable debug asserts")
set(Utils_USE_ITERATOR_DEBUG    FALSE           CACHE BOOL      "Enable state checks for iterators")
set(Utils_USE_COMPILER_CHECKS   FALSE           CACHE BOOL      "Enable compile time asserts.")
set(Utils_NO_DEBUGGER           TRUE            CACHE BOOL      "Send calls to skPrintf to printf vs skDebugger::report")


configure_file(${Utils_SOURCE_DIR}/CMake/skConfig.h.in  ${Utils_SOURCE_DIR}/Config/skConfig.h)
