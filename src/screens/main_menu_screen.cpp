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

#include "main_menu_screen.h"

#include <raygui.h>
#include <raylib.h>

#include "app/wyrmsweeper.h"
#include "gui/layout_constants.h"
#include "screens/game_screen.h"

constexpr float FONT_SIZE_TITLE = 72.F;

constexpr Vector2 GUI_BUTTON_SIZE{150.F, 60.F};

constexpr int FIELD_EASY_WIDTH      = 9;
constexpr int FIELD_EASY_HEIGHT     = 9;
constexpr int FIELD_EASY_BOMB_COUNT = 10;

constexpr int FIELD_INTERMEDIATE_WIDTH      = 16;
constexpr int FIELD_INTERMEDIATE_HEIGHT     = 16;
constexpr int FIELD_INTERMEDIATE_BOMB_COUNT = 40;

constexpr int FIELD_HARD_WIDTH      = 30;
constexpr int FIELD_HARD_HEIGHT     = 16;
constexpr int FIELD_HARD_BOMB_COUNT = 99;

MainMenuScreen::MainMenuScreen(Wyrmsweeper* game)
    : Screen(game)
    , _menuState(MenuState::Title)
{}

void MainMenuScreen::update() {}

void MainMenuScreen::render()
{
    renderBackground();

    switch (_menuState)
    {
    case MenuState::Title:
        renderTitleState();
        break;
    case MenuState::Difficulty:
        renderDifficultyState();
        break;
    }
}

void MainMenuScreen::renderTitleState()
{
    // Title
    const auto [titleWidth, titleHeight] =
        MeasureTextEx(_game->getTheme()->getFont(), "Wyrmsweeper", FONT_SIZE_TITLE, 1.F);

    const float posX = static_cast<float>(GetScreenWidth()) / 2.F - titleWidth / 2.F;
    const float posY = static_cast<float>(GetScreenHeight()) / 4.F;

    DrawTextEx(_game->getTheme()->getFont(), "Wyrmsweeper", {posX, posY}, FONT_SIZE_TITLE, 1.F,
               _game->getTheme()->getFontColor());

    // Buttons
    const float buttonY = static_cast<float>(GetScreenHeight()) - static_cast<float>(GetScreenHeight()) / 2.F;

    if (centeredButton("Play", buttonY))
    {
        _menuState = MenuState::Difficulty;
    }
    if (centeredButton("Quit", buttonY + GUI_BUTTON_SIZE.y + GUI::ITEM_SPACING))
    {
        _game->quit();
    }
}

void MainMenuScreen::renderDifficultyState()
{
    // Difficulty buttons
    constexpr float buttonCount = 3;

    constexpr float buttonsHeight = buttonCount * GUI_BUTTON_SIZE.y + (buttonCount - 1) * GUI::ITEM_SPACING;
    const float     buttonY       = static_cast<float>(GetScreenHeight()) / 2.F - buttonsHeight / 2.F;

    if (centeredButton("Easy", buttonY))
    {
        _game->setScreen(
            std::make_unique<GameScreen>(_game, FIELD_EASY_WIDTH, FIELD_EASY_HEIGHT, FIELD_EASY_BOMB_COUNT));
    }
    if (centeredButton("Advanced", buttonY + GUI_BUTTON_SIZE.y + GUI::ITEM_SPACING))
    {
        _game->setScreen(std::make_unique<GameScreen>(_game, FIELD_INTERMEDIATE_WIDTH, FIELD_INTERMEDIATE_HEIGHT,
                                                      FIELD_INTERMEDIATE_BOMB_COUNT));
    }
    if (centeredButton("Hard", buttonY + 2 * GUI_BUTTON_SIZE.y + 2 * GUI::ITEM_SPACING))
    {
        _game->setScreen(
            std::make_unique<GameScreen>(_game, FIELD_HARD_WIDTH, FIELD_HARD_HEIGHT, FIELD_HARD_BOMB_COUNT));
    }

    // Back button
    if (GuiButton({GUI::WINDOW_PADDING, GUI::WINDOW_PADDING, GUI_BUTTON_SIZE.x, GUI_BUTTON_SIZE.y}, "Back") != 0)
    {
        _menuState = MenuState::Title;
    }
}

void MainMenuScreen::renderBackground() const
{
    Rectangle destination{0.F, 0.F, 0.F, 0.F};
    destination.width  = static_cast<float>(GetScreenWidth());
    destination.height = static_cast<float>(GetScreenHeight());

    DrawTexturePro(_game->getTheme()->getBackground(), {0.F, 0.F, -1.F, -1.F}, destination, {0.F, 0.F}, 0.F, WHITE);
}

auto MainMenuScreen::centeredButton(const char* text, const float posY) -> bool
{
    const float posX = static_cast<float>(GetScreenWidth()) / 2.F - GUI_BUTTON_SIZE.x / 2.F;

    return GuiButton({posX, posY, GUI_BUTTON_SIZE.x, GUI_BUTTON_SIZE.y}, text) != 0;
}
