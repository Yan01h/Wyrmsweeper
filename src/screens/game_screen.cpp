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

#include <algorithm>
#include <raymath.h>

#include "wyrmsweeper.h"

constexpr const float ZOOM_MULTIPLIER = 0.1F;

GameScreen::GameScreen(Wyrmsweeper* game, int width, int height, int mineCount)
    : Screen(game)
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
    static bool dragCamera = false;

    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
    {
        dragCamera = true;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE))
    {
        dragCamera = false;
    }
    _camera.zoom = std::max(_camera.zoom + GetMouseWheelMove() * ZOOM_MULTIPLIER, 0.1F);

    if (dragCamera)
    {
        Vector2 diff   = Vector2Divide(GetMouseDelta(), {_camera.zoom, _camera.zoom});
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
                switch (tile.state)
                {
                case TileState::Closed:
                    source.x = CLOSED_NUM * tileSize;
                    break;
                case TileState::Open:
                    source.x = static_cast<float>(tile.number) * tileSize;
                    break;
                case TileState::Flagged:
                    source.x = FLAG_NUM * tileSize;
                    break;
                }

                source.width  = tileSize;
                source.height = tileSize;

                Rectangle destination{0.F, 0.F, 0.F, 0.F};
                destination.x      = static_cast<float>(column) * _renderTileSize - _renderFieldSize.x / 2.F;
                destination.y      = static_cast<float>(row) * _renderTileSize - _renderFieldSize.y / 2.F;
                destination.width  = _renderTileSize;
                destination.height = _renderTileSize;

                switch (tileButton(source, destination))
                {
                case MOUSE_BUTTON_LEFT:
                    handleTileLeftClick(tile, row, column);
                    break;
                case MOUSE_BUTTON_RIGHT:
                    handleTileRightClick(tile);
                    break;
                default:
                    break;
                }
            }
        }
    }
    EndMode2D();
}

void GameScreen::handleTileLeftClick(Tile& tile, int row, int column)
{
    if (tile.state == TileState::Flagged)
    {
        return;
    }

    if (tile.number == 0)
    {
        openEmtpyTilesRecursive(row, column);
    } else if (tile.state == TileState::Open)
    {
        chordClick(row, column);
    }
    tile.state = TileState::Open;
}

void GameScreen::handleTileRightClick(Tile& tile)
{
    if (tile.state == TileState::Open)
    {
        return;
    }

    if (tile.state != TileState::Flagged)
    {
        tile.state = TileState::Flagged;
    } else
    {
        tile.state = TileState::Closed;
    }
}

void GameScreen::openEmtpyTilesRecursive(int row, int column)
{
    Tile& tile = _field.getTile(row, column);
    if (tile.state == TileState::Open)
    {
        return;
    }

    tile.state = TileState::Open;
    if (tile.number == 0)
    {
        for (int blockRow = std::max(row - 1, 0); blockRow <= std::min(row + 1, _field.getHeight() - 1); blockRow++)
        {
            for (int blockColumn = std::max(column - 1, 0); blockColumn <= std::min(column + 1, _field.getWidth() - 1);
                 blockColumn++)
            {
                openEmtpyTilesRecursive(blockRow, blockColumn);
            }
        }
    }
}

void GameScreen::chordClick(int row, int column)
{
    // char bombCount = _field.countBombsAround(row, column);

    // Count flags
    int flagCount = 0;
    for (int blockRow = std::max(row - 1, 0); blockRow <= std::min(row + 1, _field.getHeight() - 1); blockRow++)
    {
        for (int blockColumn = std::max(column - 1, 0); blockColumn <= std::min(column + 1, _field.getWidth() - 1);
             blockColumn++)
        {
            if (_field.getTile(blockRow, blockColumn).state == TileState::Flagged)
            {
                flagCount++;
            }
        }
    }

    if (flagCount != _field.getTile(row, column).number)
    {
        return;
    }

    for (int blockRow = std::max(row - 1, 0); blockRow <= std::min(row + 1, _field.getHeight() - 1); blockRow++)
    {
        for (int blockColumn = std::max(column - 1, 0); blockColumn <= std::min(column + 1, _field.getWidth() - 1);
             blockColumn++)
        {
            Tile& tile = _field.getTile(blockRow, blockColumn);
            if (tile.state != TileState::Flagged)
            {
                tile.state = TileState::Open;
            }
        }
    }
}

auto GameScreen::tileButton(Rectangle& source, Rectangle& destination) const -> int
{
    int button = -1;

    float   size  = destination.width * _camera.zoom; // height is not important since its always a perfect square
    Vector2 mouse = GetMousePosition();
    Vector2 posTransform = Vector2Transform({destination.x, destination.y}, GetCameraMatrix2D(_camera));
    if (CheckCollisionPointRec(mouse, {posTransform.x, posTransform.y, size, size}))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            button = MOUSE_BUTTON_LEFT;
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            button = MOUSE_BUTTON_RIGHT;
        }
    }

    DrawTexturePro(_sheet, source, destination, {0.F, 0.F}, 0.F, WHITE);
    return button;
}