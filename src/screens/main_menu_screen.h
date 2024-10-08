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

#ifndef WS_SCREENS_MAIN_MENU_SCREEN_H
#define WS_SCREENS_MAIN_MENU_SCREEN_H

#include <cstdint>

#include "components/screen.h"

class MainMenuScreen final : public Screen
{
    enum class MenuState : uint8_t
    {
        Title = 0,
        Difficulty,
        Custom
    };
public:
    explicit MainMenuScreen(Wyrmsweeper* game);

    void update() override;
    void render() override;
private:
    // Render state functions
    void renderTitleState();
    void renderDifficultyState();
    void renderCustomState();

    void renderBackground() const;

    // State functions
    auto checkCustomValues() const -> bool;

    // GUI helper functions
    static auto centeredButton(const char* text, float posY) -> bool;
    static auto centeredSpinner(const char* text, float posY, int* val, bool editMode) -> int;
private:
    // State
    MenuState _menuState;
    int       _customWidth;
    int       _customHeight;
    int       _customBombCount;
};

#endif