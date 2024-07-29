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

#ifndef WS_SCREENS_GAME_SCREEN_H
#define WS_SCREENS_GAME_SCREEN_H

#include <raylib.h>

#include "components/mine_field.h"
#include "components/screen.h"

class GameScreen final : public Screen // NOLINT
{
    enum class GameState : uint8_t
    {
        Playing = 0,
        Won,
        Exploded
    };
public:
     GameScreen(Wyrmsweeper* game, int width, int height, int mineCount);
    ~GameScreen() override;

    void update() override;
    void render() override;
private:
    // Setup functions
    void setupCamera();
    void loadTextures();
    void loadFont();
    void calculateRenderSizes();

    // Update functions
    void updateCamera();

    // Rendering functions
    void renderBackground() const;
    void renderField();
    void renderTile(int row, int column, float tileSize);
    void renderGUI();
    void renderCenteredText(const char* text, const Color& color) const;
    void renderQuitDialog();

    // Game logic functions
    void handleTileLeftClick(const Tile& tile, int row, int column);
    void doSingleTileClick(int row, int column);
    void doChordClick(int row, int column);
    void handleTileRightClick(Tile& tile);
    void openEmtpyTilesRecursive(int row, int column);
    void explode();

    // Helper functions
    [[nodiscard]] auto tileButton(const Rectangle& source, const Rectangle& destination) const -> int;
private:
    // Game state
    GameState _gameState;
    int       _bombCount;
    int       _normalTileCount;
    bool      _quitDialog;

    // Rendering properties
    float    _renderTileSize;
    Vector2  _renderFieldSize;
    Camera2D _camera;

    // Game elements
    MineField _field;

    // Assets
    Texture2D _sheet;
    Texture2D _background;
    Font      _font;
};

#endif