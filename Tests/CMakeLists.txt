# -----------------------------------------------------------------------------
#
#   Copyright (c) 2019 Charles Carley.
#
#   Contributor(s): none yet
#
# ------------------------------------------------------------------------------
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
set(TargetName UtilsTests)
file(GLOB SRC  *.cpp)
file(GLOB TST  TestFiles/*.txt)
file(GLOB HDR  *.h)

if (NOT Utils_LIBRARY)
    set(Utils_LIBRARY ${LocalUtils_TargetName})
endif()

# Configure a config time generated *.h
# that defines the absolute path to the test directory.
# This will allow the files to be predictably loaded.
set(TestDirectory ${CMAKE_CURRENT_SOURCE_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../CMake/TestConfig.h.in  
               ${CMAKE_CURRENT_BINARY_DIR}/TestConfig.h)

include_directories(. ${CMAKE_CURRENT_BINARY_DIR} ${Utils_INCLUDE})

add_executable(${TargetName} catch/catch.hpp ${SRC} ${HDR} ${TST})
target_link_libraries(${TargetName} ${Utils_LIBRARY})

set_target_properties(${TargetName} PROPERTIES FOLDER "Units")

if (Utils_AUTO_RUN_TESTS)

    add_custom_command(TARGET 
        ${TargetName} POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND  $<TARGET_FILE:${TargetName}>
    )
endif()

if (MSVC)
    # Relative to the root directory where make is called
    set_target_properties(
        ${TargetName} 
        PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY 
        ${Utils_SOURCE_DIR}
    )
endif()
