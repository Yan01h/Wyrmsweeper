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

find_package(raylib QUIET CONFIG)
if (NOT raylib_FOUND)
    include(FetchContent)
	message(STATUS "Raylib not found! Fetching raylib from github.")
	set(BUILD_EXAMPLES OFF CACHE BOOL "")
    set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "")
	FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        174313acbfa74d2a45fc0207f47ab00fbdb06050
    )
    message(STATUS "Downloading raylib...")
    FetchContent_MakeAvailable(raylib)
    message(STATUS "Downloading raylib... Done")
else ()
    message(STATUS "Raylib include dir: ${raylib_INCLUDE_DIR}")
    message(STATUS "Raylib libraries:   ${raylib_LIBRARIES}")
endif ()