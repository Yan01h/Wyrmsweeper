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

cmake_minimum_required(VERSION 3.15...3.29)

message(STATUS "-------------------------------")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "-------------------------------")
message(STATUS "CMake SrcDir: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMake BinDir: ${CMAKE_BINARY_DIR}")

if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW) # Project Version
endif ()
if (POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW) # MSVC Runtime Library
endif ()

##################
#    Settings    #
##################

# For clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Disable in-source builds
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Set C/C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Other settings
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWS_DEBUG_BUILD")

set(WS_THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparties")

#################
#    Project    #
#################

project(
    Wyrmsweeper
    VERSION 0.1.0
    DESCRIPTION "A simple C++ Minesweeper clone"
    HOMEPAGE_URL "https://github.com/Yan01h/Wyrmsweeper"
    LANGUAGES CXX
)

# Version
set(WS_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(WS_VERSION_STRING "${WS_VERSION_STRING}-debug")
endif ()

# Other project settings
if (MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else ()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif ()
endif ()

########################
#    Subdirectories    #
########################

add_subdirectory(src)