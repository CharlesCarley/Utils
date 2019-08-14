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
    skHash.cpp
    skMemoryStream.cpp
    skMemoryUtils.cpp
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
    skBinarySearchTree.h
    skDebugger.h
    skDelegate.h
    skDictionary.h
    skDeque.h
    skFileStream.h
    skFixedArray.h
    skFixedString.h
    skHash.h
    skLinkedList.h
    skList.h
    skMap.h
    skMemoryStream.h
    skMemoryUtils.h
    skMinMax.h
    skParser.h
    skPlatformHeaders.h
    skQueue.h
    skRandom.h
    skSingleton.h
    skSort.h
    skStack.h
    skStackQueue.h
    skStreams.h
    skString.h
    skStringConverter.h
    skTimer.h
    skTraits.h
    skUserObject.h
    skValue.h

    Config/skConfig.h
    CMake/skConfig.h.in
    CMake/ConfigureUtils.cmake
)


include_directories(../ .)
add_library(Utils ${Utils_SRC} ${Utils_HDR})

if (USE_FOLDERS)
    set_target_properties(Utils PROPERTIES FOLDER "${FOLDER_NAME}")
endif()
