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
#include <raygui.h>
#include <raymath.h>

#include "screens/main_menu_screen.h"
#include "wyrmsweeper.h"

namespace {
constexpr float ZOOM_MULTIPLIER    = 0.1F;
constexpr float MINIMUM_ZOOM_LEVEL = 0.1F;

constexpr int   FONT_TTF_DEFAULT_NUMCHARS = 95;
constexpr int   FONT_LOAD_FONT_SIZE       = 32;
constexpr float FONT_SIZE_BIG             = 48.F;

constexpr float GUI_BUTTON_SIZE        = 50.F;
constexpr float GUI_QUIT_DIALOG_WIDTH  = 500.F;
constexpr float GUI_QUIT_DIALOG_HEIGHT = 150.F;
} // namespace

GameScreen::GameScreen(Wyrmsweeper* game, int width, int height, int mineCount)
    : Screen(game)
    , _gameState(GameState::Playing)
    , _bombCount(mineCount)
    , _normalTileCount(width * height - mineCount)
    , _quitDialog(false)
    , _renderTileSize(0.F)
    , _renderFieldSize()
    , _camera()
    , _field(width, height, mineCount)
    , _sheet()
    , _background()
    , _font()
{
    setupCamera();
    loadTextures();
    loadFont();
    calculateRenderSizes();

    TraceLog(LOG_INFO, "GameScreen(0x%2x) constructed", this);
}

GameScreen::~GameScreen()
{
    UnloadTexture(_sheet);
    UnloadTexture(_background);

    UnloadFont(_font);

    GuiSetFont(GetFontDefault());

    TraceLog(LOG_INFO, "GameScreen(0x%2x) destroyed", this);
}

void GameScreen::update()
{
    if (_quitDialog)
    {
        return;
    }

    updateCamera();
}

void GameScreen::render()
{
    _camera.offset.x = static_cast<float>(GetScreenWidth()) / 2.F;
    _camera.offset.y = static_cast<float>(GetScreenHeight()) / 2.F;

    renderBackground();
    renderField();
    renderGUI();
}

void GameScreen::setupCamera()
{
    _camera.target = {0, 0};
    _camera.zoom   = 1.F;
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

void GameScreen::loadFont()
{
    _font = LoadFontFromMemory(".ttf", _game->getCurrentTheme()->getFontData(),
                               sizeof(_game->getCurrentTheme()->getFontData()), FONT_LOAD_FONT_SIZE, nullptr,
                               FONT_TTF_DEFAULT_NUMCHARS);
    GuiSetFont(_font);
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);
    GuiSetIconScale(2);
}

void GameScreen::calculateRenderSizes()
{
    const auto tileWidth  = static_cast<float>(GetScreenWidth()) / static_cast<float>(_field.getWidth());
    const auto tileHeight = static_cast<float>(GetScreenHeight()) / static_cast<float>(_field.getHeight());
    _renderTileSize       = std::min(tileWidth, tileHeight);

    _renderFieldSize.x = static_cast<float>(_field.getWidth()) * _renderTileSize;
    _renderFieldSize.y = static_cast<float>(_field.getHeight()) * _renderTileSize;
}

void GameScreen::updateCamera()
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
    _camera.zoom = std::max(_camera.zoom + GetMouseWheelMove() * ZOOM_MULTIPLIER, MINIMUM_ZOOM_LEVEL);

    if (dragCamera)
    {
        const Vector2 diff = Vector2Divide(GetMouseDelta(), {_camera.zoom, _camera.zoom});
        _camera.target     = Vector2Subtract(_camera.target, diff);
    }
}

void GameScreen::renderBackground() const
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
    const auto tileSize = static_cast<float>(_game->getCurrentTheme()->getTileSize());

    BeginMode2D(_camera);
    {
        for (int row = 0; row < _field.getHeight(); row++)
        {
            for (int column = 0; column < _field.getWidth(); column++)
            {
                renderTile(row, column, tileSize);
            }
        }
    }
    EndMode2D();
}

void GameScreen::renderTile(const int row, const int column, const float tileSize)
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

void GameScreen::renderGUI()
{
    if (_gameState == GameState::Exploded)
    {
        renderCenteredText("Game Over!", RED);
    } else if (_gameState == GameState::Won)
    {
        renderCenteredText("You Win!", GREEN);
    }

    if (GuiButton({10, 10, GUI_BUTTON_SIZE, GUI_BUTTON_SIZE}, "#56#") != 0) // NOLINT
    {
        _quitDialog = true;
    }
    if (_quitDialog)
    {
        GuiSetIconScale(1); // Needed so info icon won't render outside of the dialog window
        const Vector2 pos{static_cast<float>(GetScreenWidth()) / 2 - GUI_QUIT_DIALOG_WIDTH / 2,
                          static_cast<float>(GetScreenHeight()) / 2 - GUI_QUIT_DIALOG_HEIGHT / 2};

        const int result = GuiMessageBox({pos.x, pos.y, GUI_QUIT_DIALOG_WIDTH, GUI_QUIT_DIALOG_HEIGHT}, "#191#",
                                         "Are you sure you want to return to the main menu?", "Cancel;Yes");
        GuiSetIconScale(2);

        switch (result)
        {
        case 0:
        case 1:
            _quitDialog = false;
            break;
        case 2:
            _game->changeScreen(std::make_unique<MainMenuScreen>(_game));
            return;
        default:
            break;
        }
    }

    if (_gameState != GameState::Playing)
    {
        if (GuiButton({20 + GUI_BUTTON_SIZE, 10, GUI_BUTTON_SIZE, GUI_BUTTON_SIZE}, "#60#")) // NOLINT
        {
            _game->changeScreen(
                std::make_unique<GameScreen>(_game, _field.getWidth(), _field.getHeight(), _field.getBombCount()));
        }
    }
}

void GameScreen::renderCenteredText(const char* text, const Color& color) const
{
    auto [x, y] = MeasureTextEx(_font, text, FONT_SIZE_BIG, 1.F);
    const Vector2 pos{static_cast<float>(GetScreenWidth()) / 2 - x / 2, static_cast<float>(GetScreenHeight()) - 2 * y};

    DrawTextEx(_font, text, pos, FONT_SIZE_BIG, 1.F, color);
}

void GameScreen::handleTileLeftClick(const Tile& tile, const int row, const int column)
{
    if (tile.state == TileState::Open && tile.number != 0)
    {
        doChordClick(row, column);
    } else
    {
        doSingleTileClick(row, column);
    }
}

void GameScreen::doSingleTileClick(const int row, const int column)
{
    auto& [number, state] = _field.getTile(row, column);
    if (state == TileState::Open || state == TileState::Flagged)
    {
        return;
    }

    if (number == 0)
    {
        openEmtpyTilesRecursive(row, column);
    } else
    {
        if (number != BOMB_NUM)
        {
            _normalTileCount--;
        }
        state = TileState::Open;
    }

    if (number == BOMB_NUM)
    {
        explode();
    }

    if (_normalTileCount == 0)
    {
        _gameState = GameState::Won;
    }
}

void GameScreen::doChordClick(const int row, const int column)
{
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
            doSingleTileClick(blockRow, blockColumn);
        }
    }
}

void GameScreen::handleTileRightClick(Tile& tile) // NOLINT
{
    if (tile.state == TileState::Open)
    {
        return;
    }
    tile.state = tile.state == TileState::Closed ? TileState::Flagged : TileState::Closed;
}

void GameScreen::openEmtpyTilesRecursive(const int row, const int column) // NOLINT
{
    auto& [number, state] = _field.getTile(row, column);
    if (state == TileState::Open)
    {
        return;
    }

    state = TileState::Open;
    _normalTileCount--;
    if (number == 0)
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

auto GameScreen::tileButton(const Rectangle& source, const Rectangle& destination) const -> int
{
    int button = -1;

    const float   size  = destination.width * _camera.zoom; // height is not important since its always a perfect square
    const Vector2 mouse = GetMousePosition();
    if (const auto [x, y] = Vector2Transform({destination.x, destination.y}, GetCameraMatrix2D(_camera));
        _gameState == GameState::Playing && !_quitDialog && CheckCollisionPointRec(mouse, {x, y, size, size}))
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

void GameScreen::explode()
{
    _gameState = GameState::Exploded;
    for (int row = 0; row < _field.getHeight(); row++)
    {
        for (int column = 0; column < _field.getWidth(); column++)
        {
            if (auto& [number, state] = _field.getTile(row, column); number == BOMB_NUM)
            {
                state = TileState::Open;
            }
        }
    }
}