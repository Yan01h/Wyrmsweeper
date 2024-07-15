/*
 * MIT License
 *
 * Copyright (c) 2024 Yan01h
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "wyrmsweeper.h"

#include <cassert>
#include <raylib.h>

#include "screens/game_screen.h" // Temporary
#include "screens/main_menu_screen.h"
#include "themes/classic_theme.h"

const int DEFAULT_SCREEN_WIDHT  = 1280;
const int DEFAULT_SCREEN_HEIGHT = 720;

void Wyrmsweeper::run()
{
#ifndef WS_DEBUG_BUILD
    SetTraceLogLevel(LOG_NONE);
#endif

    /*    Init raylib    */
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

    InitWindow(DEFAULT_SCREEN_WIDHT, DEFAULT_SCREEN_HEIGHT, "Wyrmsweeper");
    SetExitKey(KEY_NULL);

    /*    Init game    */
    _currentScreen = std::make_unique<MainMenuScreen>(this);
    _currentTheme  = std::make_unique<ClassicTheme>();

    /*    Main loop    */
    while (!WindowShouldClose())
    {
        _currentScreen->update();

        BeginDrawing();
        ClearBackground(BLACK);

        _currentScreen->render();

        EndDrawing();
    }

    /*    Cleanup raylib    */
    CloseWindow();
}

void Wyrmsweeper::changeScreen(std::unique_ptr<Screen> newScreen)
{
    assert(newScreen.get() != nullptr);
    _currentScreen = std::move(newScreen);
}

auto Wyrmsweeper::getCurrentTheme() -> Theme*
{
    return _currentTheme.get();
}