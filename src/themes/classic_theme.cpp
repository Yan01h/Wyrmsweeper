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

#include "assets/classic_theme/sprite_sheet.h"

auto ClassicTheme::getSpriteSheetWidth() -> int
{
    return Assets::Classic::SHEET_WIDTH;
}

auto ClassicTheme::getSpriteSheetHeight() -> int
{
    return Assets::Classic::SHEET_HEIGHT;
}

auto ClassicTheme::getSpriteSheetData() -> void*
{
    return (void*)Assets::Classic::SHEET_DATA; // NOLINT
}

auto ClassicTheme::getBackgroundWidth() -> int
{
    int backgroundWidth = 1;
    return backgroundWidth;
}

auto ClassicTheme::getBackgroundHeight() -> int
{
    int backgroundHeight = 1;
    return backgroundHeight;
}

auto ClassicTheme::getBackgroundData() -> void*
{
    static unsigned int backgroundData[] = {0xffc0c0c0}; // NOLINT
    return (void*)backgroundData;
}
