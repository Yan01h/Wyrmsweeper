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
        app/wyrmsweeper.h
        app/wyrmsweeper.cpp
        assets/classic_theme/font.h
        assets/classic_theme/sprite_sheet.h
        components/mine_field.h
        components/mine_field.cpp
        components/screen.h
        components/screen.cpp
        components/theme.h
        gui/layout_constants.h
        screens/game_screen.h
        screens/game_screen.cpp
        screens/main_menu_screen.h
        screens/main_menu_screen.cpp
        themes/classic_theme.h
        themes/classic_theme.cpp
        thirdparties/raygui.cpp
        thirdparties/raylib_utils.h
        thirdparties/raylib_utils.cpp
        main.cpp
)

if (WIN32)
    set(WS_SOURCE_FILES ${WS_SOURCE_FILES} win32/resource.rc)
endif ()

######################
#    Thirdparties    #
######################

find_package(raylib REQUIRED)
find_package(raygui REQUIRED)

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
            LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS"
    )
else ()
    target_compile_options(Wyrmsweeper PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()

# Include paths
target_include_directories(Wyrmsweeper PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/generated)

# Libraries
target_link_libraries(Wyrmsweeper PRIVATE raylib raygui)

# Configuration
configure_file(cmake_config.h.in ${CMAKE_BINARY_DIR}/generated/cmake_config.h)