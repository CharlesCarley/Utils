# Utils

Utils is a small general purpose utility library in C++.


## Building

Building with CMAKE and make.

```txt
mkdir Build
cd Build
cmake ..
make
```

Optional defines.

+ Utils_USE_DEBUG_ASSERT - enable / disable assert in DEBUG builds. Default: ON
+ Utils_USE_ITERATOR_DEBUG - enable / disable extra iterator checks in DEBUG builds. Default: OFF
+ Utils_USE_COMPILER_CHECKS - enable / disable compile asserts. Default: OFF
+ Utils_NO_DEBUGGER - send calls to skPrintf to printf vs skDebugger::report. Default: ON
+ Utils_BUILD_TESTS - build basic tests. Default: OFF
+ Utils_AUTO_RUN_TESTS - Adds a custom target that runs on build.
