# ------------------------------------------------------------------------------
#   $Id: CMakeLists.txt 9 2019-03-09 00:08:08Z Charlie $
#
#   Copyright (c) Charles Carley.
#   
#   All Rights Reserved.
#
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)


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
)


include_directories(..)
add_library(Utils ${Utils_SRC} ${Utils_HDR})
