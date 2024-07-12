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

#include "game_screen.h"

#include <raylib.h>

#include "debug/log.h"
#include "wyrmsweeper.h"

constexpr const float ZOOM_MULTIPLIER = 0.1F;

GameScreen::GameScreen(Wyrmsweeper* game, int width, int height, int mineCount)
    : Screen(game)
    , _camera()
    , _field(width, height, mineCount)
{
    _camera.target = {0, 0};
    _camera.zoom   = 1.F;
}

void GameScreen::update()
{
    static bool dragCamera = false;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        dragCamera = true;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        dragCamera = false;
    }
    _camera.zoom += GetMouseWheelMove() * ZOOM_MULTIPLIER;

    if (dragCamera)
    {
        Vector2 diff = GetMouseDelta();
        _camera.target.x -= diff.x / _camera.zoom;
        _camera.target.y -= diff.y / _camera.zoom;
    }
}

void GameScreen::render()
{
    BeginMode2D(_camera);
    {
        DrawRectangle(0, 0, 300, 300, RED); // NOLINT
    }
    EndMode2D();
}