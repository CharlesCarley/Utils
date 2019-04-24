cmake_minimum_required(VERSION 3.0)
project(Utils)

list(APPEND CMAKE_MODULE_PATH 
    ${Utils_SOURCE_DIR}/CMake
    )

include(ConfigureUtils)


set(Utils_SRC
    skAssert.cpp
    skDebugger.cpp
    skFileStream.cpp
    skLogger.cpp
    skMemoryStream.cpp
    skMemoryUtils.cpp
    skStreams.cpp
    skString.cpp
    skStringConverter.cpp
    skRandom.cpp
    skTimer.cpp
    skParser.cpp
)

set(Utils_HDR
    skAllocator.h
    skArray.h
    skAssert.h
    skDebugger.h
    skDelegate.h
    skDeque.h
    skEndian.h
    skFileStream.h
    skFixedArray.h
    skFixedString.h
    skHash.h
    skList.h
    skLogger.h
    skMap.h
    skMemoryStream.h
    skMemoryUtils.h
    skMinMax.h
    skPlatformHeaders.h
    skPointer.h
    skSingleton.h
    skSort.h
    skStack.h
    skStreams.h
    skString.h
    skStringConverter.h
    skRandom.h
    skTimer.h
    skTraits.h
    skTypes.h
    skUserObject.h
    skValue.h
    skParser.h
    Config/skConfig.h
    CMake/skConfig.h.in
    CMake/ConfigureUtils.cmake
)


include_directories(../ .)
add_library(Utils ${Utils_SRC} ${Utils_HDR})
