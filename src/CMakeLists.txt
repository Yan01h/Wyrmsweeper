# MIT License
# 
# Copyright (c) 2024 Yan01h
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

#################
#    Sources    #
#################

set(
    WS_SOURCE_FILES
    core/wyrmsweeper.h
    core/wyrmsweeper.cpp
    debug/log.h
    main.cpp
)

######################
#    Thirdparties    #
######################

include(FetchContent)

# spdlog
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "")
set(SPDLOG_INSTALL OFF CACHE BOOL "")
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        d276069a6e916b1e1fd45885b15b72bd8ee000a7
)
message(STATUS "Making spdlog available...")
FetchContent_MakeAvailable(spdlog)
message(STATUS "Making spdlog available... Done")

####################
#    Executable    #
####################

add_executable(Wyrmsweeper ${WS_SOURCE_FILES})

if (WIN32)
    target_compile_definitions(Wyrmsweeper PRIVATE WS_PLATFORM_WINDOWS)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Wyrmsweeper PRIVATE /W4 /WX)
    set_target_properties(
        Wyrmsweeper PROPERTIES
        LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS"
    )
else ()
    target_compile_options(Wyrmsweeper PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()

# Include paths
target_include_directories(Wyrmsweeper PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/generated)

# Libraries
target_link_libraries(Wyrmsweeper PRIVATE spdlog)

# Configuration
configure_file(cmake_config.h.in ${CMAKE_BINARY_DIR}/generated/cmake_config.h)