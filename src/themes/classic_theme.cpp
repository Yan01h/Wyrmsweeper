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

#include "classic_theme.h"

#include <raylib.h>

#include "assets/classic_theme/font.h"
#include "assets/classic_theme/sprite_sheet.h"
#include "thirdparties/raylib_utils.h"

static constexpr unsigned int BACKGROUND_DATA[] = {0xffc0c0c0};

ClassicTheme::ClassicTheme()
    : _spriteSheet()
    , _background()
    , _font()
{
    loadAssets();

    TraceLog(LOG_INFO, "Classic theme loaded!");
}

ClassicTheme::~ClassicTheme()
{
    unloadAssets();

    TraceLog(LOG_INFO, "Classic theme unloaded!");
}

auto ClassicTheme::getTileSize() const -> int
{
    return Assets::Classic::SHEET_HEIGHT;
}

auto ClassicTheme::getSpriteSheet() const -> const Texture2D&
{
    return _spriteSheet;
}

auto ClassicTheme::getBackground() const -> const Texture2D&
{
    return _background;
}

auto ClassicTheme::getFont() const -> const Font&
{
    return _font;
}

void ClassicTheme::loadAssets()
{
    _spriteSheet = RaylibUtils::loadTextureFromMemory(Assets::Classic::SHEET_WIDTH, Assets::Classic::SHEET_HEIGHT,
                                                      Assets::Classic::SHEET_DATA);

    _font = RaylibUtils::loadFontFromMemory(Assets::Classic::FONT_DATA, sizeof(Assets::Classic::FONT_DATA));

    createAndLoadBackground();
}

void ClassicTheme::unloadAssets() const
{
    UnloadTexture(_spriteSheet);
    UnloadTexture(_background);

    UnloadFont(_font);
}

void ClassicTheme::createAndLoadBackground()
{
    // The background of the classic theme only has one color so we can simply create a 1x1 texture
    constexpr int width  = 1;
    constexpr int height = 1;

    _background = RaylibUtils::loadTextureFromMemory(width, height, BACKGROUND_DATA);
}
