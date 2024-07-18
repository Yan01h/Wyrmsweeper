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

#include <raymath.h>

#include "wyrmsweeper.h"

constexpr const float ZOOM_MULTIPLIER = 0.1F;

GameScreen::GameScreen(Wyrmsweeper* game, int width, int height, int mineCount)
    : Screen(game)
    , _dragCamera(false)
    , _renderTileSize(0.F)
    , _renderFieldSize()
    , _camera()
    , _field(width, height, mineCount)
    , _sheet()
    , _background()
{
    _camera.target = {0, 0};
    _camera.zoom   = 1.F;

    loadTextures();

    auto tileWidth  = static_cast<float>(GetScreenWidth()) / static_cast<float>(_field.getWidth());
    auto tileHeight = static_cast<float>(GetScreenHeight()) / static_cast<float>(_field.getHeight());
    _renderTileSize = std::min(tileWidth, tileHeight);

    _renderFieldSize.x = static_cast<float>(_field.getWidth()) * _renderTileSize;
    _renderFieldSize.y = static_cast<float>(_field.getHeight()) * _renderTileSize;
}

void GameScreen::update()
{
    Vector2 mouseDelta = GetMouseDelta();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (mouseDelta.x != 0.F || mouseDelta.y != 0.F))
    {
        _dragCamera = true;
    }

    _camera.zoom = std::max(_camera.zoom + GetMouseWheelMove() * ZOOM_MULTIPLIER, 0.1F);

    if (_dragCamera)
    {
        Vector2 diff   = Vector2Divide(mouseDelta, {_camera.zoom, _camera.zoom});
        _camera.target = Vector2Subtract(_camera.target, diff);
    }
}

void GameScreen::render()
{
    _camera.offset.x = static_cast<float>(GetScreenWidth()) / 2.F;
    _camera.offset.y = static_cast<float>(GetScreenHeight()) / 2.F;

    renderBackground();
    renderField();
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

void GameScreen::renderBackground()
{
    Rectangle source{0.F, 0.F, 0.F, 0.F};
    source.width  = static_cast<float>(_game->getCurrentTheme()->getBackgroundWidth());
    source.height = static_cast<float>(_game->getCurrentTheme()->getBackgroundHeight());

    Rectangle destination{0.F, 0.F, 0.F, 0.F};
    destination.width  = static_cast<float>(GetScreenWidth());
    destination.height = static_cast<float>(GetScreenHeight());

    DrawTexturePro(_background, source, destination, {0.F, 0.F}, 0.F, WHITE);
}

void GameScreen::renderField()
{
    auto tileSize = static_cast<float>(_game->getCurrentTheme()->getTileSize());

    BeginMode2D(_camera);
    {
        for (int row = 0; row < _field.getHeight(); row++)
        {
            for (int column = 0; column < _field.getWidth(); column++)
            {
                Tile& tile = _field.getTile(row, column);

                Rectangle source{0.F, 0.F, 0.F, 0.F};
                source.x      = tile.open ? static_cast<float>(tile.number) * tileSize : CLOSED_NUM * tileSize;
                source.width  = tileSize;
                source.height = tileSize;

                Rectangle destination{0.F, 0.F, 0.F, 0.F};
                destination.x      = static_cast<float>(column) * _renderTileSize - _renderFieldSize.x / 2.F;
                destination.y      = static_cast<float>(row) * _renderTileSize - _renderFieldSize.y / 2.F;
                destination.width  = _renderTileSize;
                destination.height = _renderTileSize;

                if (tileButton(source, destination))
                {
                    handleTileClick(tile);
                }
            }
        }
    }
    EndMode2D();
}

void GameScreen::handleTileClick(Tile& tile)
{
    tile.open = true;
}

auto GameScreen::tileButton(Rectangle& source, Rectangle& destination) -> bool
{
    bool clicked = false;

    float   size  = destination.width * _camera.zoom; // height is not important since its always a perfect square
    Vector2 mouse = GetMousePosition();
    Vector2 posTransform = Vector2Transform({destination.x, destination.y}, GetCameraMatrix2D(_camera));
    if (CheckCollisionPointRec(mouse, {posTransform.x, posTransform.y, size, size}))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (!_dragCamera)
            {
                clicked = true;
            } else
            {
                _dragCamera = false;
            }
        }
    }

    DrawTexturePro(_sheet, source, destination, {0.F, 0.F}, 0.F, WHITE);
    return clicked;
}