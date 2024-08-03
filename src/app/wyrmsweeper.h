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

#ifndef WS_APP_WYRMSWEEPER_H
#define WS_APP_WYRMSWEEPER_H

#include <memory>

#include "components/screen.h"
#include "components/theme.h"

class Wyrmsweeper final
{
public:
    Wyrmsweeper() = default;

    void run();
    void quit();

    void setScreen(std::unique_ptr<Screen> newScreen);

    [[nodiscard]] auto getTheme() const -> ITheme*;
    [[nodiscard]] auto getAutoChordSetting() -> bool&;
private:
    bool _running   = true;
    bool _autoChord = false;

    std::unique_ptr<Screen> _currentScreen;
    std::unique_ptr<Screen> _nextScreen;

    std::unique_ptr<ITheme> _currentTheme;
};

#endif