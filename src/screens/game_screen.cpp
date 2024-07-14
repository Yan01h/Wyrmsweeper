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

#include "assets/classic_theme/sprite_sheet.h" // temporary
#include "debug/log.h"
#include "wyrmsweeper.h"

constexpr const float ZOOM_MULTIPLIER = 0.1F;

// TODO: Remove all the this:
// BACKGROUND: 192 192 192 255
// constexpr const int TILE_BOX_SIZE   = 100;
// constexpr const int TILE_BOX_BORDER = 10;
//
// constexpr const Color TILE_COLOR_BORDER = {80, 80, 80, 255};
// constexpr const Color TILE_COLOR_CENTER = {130, 130, 130, 255};

GameScreen::GameScreen(Wyrmsweeper* game, int width, int height, int mineCount)
    : Screen(game)
    , _camera()
    , _field(width, height, mineCount)
    , _sheet()
{
    _camera.target = {0, 0};
    _camera.zoom   = 1.F;

    // Load sprite sheet
    Image spriteSheet;
    spriteSheet.width   = Themes::Classic::SHEET_WIDTH;
    spriteSheet.height  = Themes::Classic::SHEET_HEIGHT;
    spriteSheet.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    spriteSheet.mipmaps = 1;
    spriteSheet.data    = (void*)Themes::Classic::SHEET_DATA; // NOLINT

    _sheet = LoadTextureFromImage(spriteSheet);
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
        for (int row = 0; row < _field.getHeight(); row++)
        {
            for (int column = 0; column < _field.getWidth(); column++)
            {
                Tile& tile = _field.get(row, column);
                if (tile.open)
                {
                    // TODO
                } else
                {
                    Rectangle src{CLOSED_NUM * Themes::Classic::SHEET_HEIGHT, 0, Themes::Classic::SHEET_HEIGHT,
                                  Themes::Classic::SHEET_HEIGHT};
                    Vector2   pos{(float)column * Themes::Classic::SHEET_HEIGHT,
                                (float)row * Themes::Classic::SHEET_HEIGHT};
                    DrawTextureRec(_sheet, src, pos, WHITE);
                }
            }
        }
    }
    EndMode2D();
}