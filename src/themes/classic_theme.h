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

#ifndef WS_THEMES_CLASSIC_THEME_H
#define WS_THEMES_CLASSIC_THEME_H

#include "components/theme.h"

class ClassicTheme final : public ITheme
{
public:
     ClassicTheme();
    ~ClassicTheme() override;

    [[nodiscard]] auto getTileSize() const -> int override;

    [[nodiscard]] auto getSpriteSheet() const -> const Texture2D& override;
    [[nodiscard]] auto getBackground() const -> const Texture2D& override;
    [[nodiscard]] auto getFont() const -> const Font& override;

    [[nodiscard]] auto getFontColor() const -> const Color override;
private:
    void loadAssets();
    void unloadAssets() const;

    void createAndLoadBackground();
private:
    Texture2D _spriteSheet;
    Texture2D _background;
    Font      _font;
};

#endif