# -----------------------------------------------------------------------------
#   Copyright (c) 2019 Charles Carley.
#
#   This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
#   Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
# ------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)
project(Utils)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(Utils_USE_DEBUG_ASSERT      "Enable debug asserts" ON)
option(Utils_USE_ITERATOR_DEBUG    "Enable state checks for iterators" OFF)
option(Utils_USE_COMPILER_CHECKS   "Enable compile time asserts." OFF)
option(Utils_NO_DEBUGGER           "Send calls to skPrintf to printf vs skDebugger::report" ON)
option(Utils_BUILD_TESTS           "Build basic tests" OFF)
option(Utils_AUTO_RUN_TESTS        "Adds a custom target that runs on build" OFF)

configure_file(${Utils_SOURCE_DIR}/CMake/skConfig.h.in  ${Utils_SOURCE_DIR}/Config/skConfig.h)


# Build configuration, for example:
# set(Utils_ExternalTarget TRUE)
# set(Utils_TargetFolders  TRUE)
# set(Utils_TargetName     SpecificTargetName)
# set(Utils_TargetGroup    SpecificTargetGroup)
# subdirs(Utils)

if (Utils_ExternalTarget)
    set(TargetFolders ${Utils_TargetFolders})
    set(TargetName    ${Utils_TargetName})
    set(TargetGroup   ${Utils_TargetGroup})
else()
    set(TargetFolders FALSE)
    set(TargetName Utils)
    unset(TargetGroup )
endif()



if (MSVC)
    source_group("Config\\Workflows"  FILES .github/workflows/ccpp.yml)
    source_group("Config\\CMake"      FILES  CMake/skConfig.h.in)
    source_group("Config\\Markdown"   FILES  README.md)
endif()

set(Utils_EXT
    .github/workflows/ccpp.yml
    CMake/skConfig.h.in
    README.md
)

set(Utils_SRC
    skAssert.cpp
    skChar.cpp
    skHexPrint.cpp
    skDebugger.cpp
    skFileStream.cpp
    skHash.cpp
    skLogger.cpp
    skMemoryStream.cpp
    skMemoryUtils.cpp
    skString.cpp
    skStringBuilder.cpp
    skStreams.cpp
    skRandom.cpp
    skTimer.cpp
    CommandLine/skCommandLineParser.cpp
    CommandLine/skCommandLineScanner.cpp
)

set(Utils_HDR
    skAllocator.h
    skChar.h
    skHexPrint.h
    skArrayBase.h
    skArray.h
    skAssert.h
    skBinarySearchTree.h
    skDebugger.h
    skDelegate.h
    skDictionary.h
    skDeque.h
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
    skQueue.h
    skRandom.h
    skSort.h
    skSingleton.h
    skStack.h
    skStreams.h
    skString.h
    skStringBuilder.h
    skStringConverter.h
    skTimer.h
    skTraits.h
    skUserObject.h
    skValue.h
    Config/skConfig.h
    CommandLine/skCommandLineOption.h
    CommandLine/skCommandLineParser.h
    CommandLine/skCommandLineScanner.h
    CommandLine/skCommandLineToken.h
)


include_directories(../ .)
add_library(${TargetName} ${Utils_SRC} ${Utils_HDR} ${Utils_EXT})

if (Utils_BUILD_TESTS)
    set(LocalUtils_TargetName ${TargetName})
    subdirs(Tests)
endif()

if (TargetFolders)
    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
endif()
