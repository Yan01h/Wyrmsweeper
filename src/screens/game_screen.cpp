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

#include "wyrmsweeper.h"

constexpr const float ZOOM_MULTIPLIER = 0.1F;

GameScreen::GameScreen(Wyrmsweeper* game, int width, int height, int mineCount)
    : Screen(game)
    , _camera()
    , _field(width, height, mineCount)
    , _sheet()
    , _background()
    , _bgSrc({0.F, 0.F, 0.F, 0.F})
    , _bgDest({0.F, 0.F, 0.F, 0.F})
    , _spriteSrc({0.F, 0.F, 0.F, 0.F})
    , _spriteDest({0.F, 0.F, 0.F, 0.F})
{
    _camera.target = {0, 0};
    _camera.zoom   = 1.F;

    loadTextures();
    setupRenderRecs();
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
    auto tileSize = static_cast<float>(_game->getCurrentTheme()->getTileSize()); // Avoid repeating call to getTileSize

    // Backgorund
    DrawTexturePro(_background, _bgSrc, _bgDest, {0.F, 0.F}, 0.F, WHITE);

    // Field
    BeginMode2D(_camera);
    {
        for (int row = 0; row < _field.getHeight(); row++)
        {
            for (int column = 0; column < _field.getWidth(); column++)
            {
                Tile& tile = _field.get(row, column);

                _spriteSrc.x  = tile.open ? static_cast<float>(tile.number) * tileSize : CLOSED_NUM * tileSize;
                _spriteDest.x = static_cast<float>(column) * tileSize;
                _spriteDest.y = static_cast<float>(row) * tileSize;

                DrawTexturePro(_sheet, _spriteSrc, _spriteDest, {0.F, 0.F}, 0.F, WHITE);
            }
        }
    }
    EndMode2D();
}

void GameScreen::loadTextures()
{
    // Load sprite sheet
    Image spriteSheet{};
    spriteSheet.width   = _game->getCurrentTheme()->getSpriteSheetWidth();
    spriteSheet.height  = _game->getCurrentTheme()->getSpriteSheetHeight();
    spriteSheet.data    = _game->getCurrentTheme()->getSpriteSheetData();
    spriteSheet.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; // All sprite sheets must have this format
    spriteSheet.mipmaps = 1;

    _sheet = LoadTextureFromImage(spriteSheet);

    // Load background
    Image background{};
    background.width   = _game->getCurrentTheme()->getBackgroundWidth();
    background.height  = _game->getCurrentTheme()->getBackgroundHeight();
    background.data    = _game->getCurrentTheme()->getBackgroundData();
    background.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; // All backgrounds must have this format
    background.mipmaps = 1;

    _background = LoadTextureFromImage(background);
}

void GameScreen::setupRenderRecs()
{
    // Background
    _bgSrc.width  = static_cast<float>(_game->getCurrentTheme()->getBackgroundWidth());
    _bgSrc.height = static_cast<float>(_game->getCurrentTheme()->getBackgroundHeight());

    _bgDest.width  = static_cast<float>(GetScreenWidth());
    _bgDest.height = static_cast<float>(GetScreenHeight());

    // Sprite sheet
    auto tileSize = static_cast<float>(_game->getCurrentTheme()->getTileSize());

    _spriteSrc.width  = tileSize;
    _spriteSrc.height = tileSize;

    _spriteDest.width  = tileSize;
    _spriteDest.height = tileSize;
}