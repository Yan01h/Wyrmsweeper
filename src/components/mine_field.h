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

#ifndef WS_COMPONENTS_MINE_FIELD_H
#define WS_COMPONENTS_MINE_FIELD_H

#include <vector>

constexpr char BOMB_NUM   = 9;
constexpr char CLOSED_NUM = 10;
constexpr char FLAG_NUM   = 11;

struct Tile
{
    char number;
    bool open;
};

class MineField final
{
public:
    MineField() = delete;
    MineField(int width, int height, int mineCount);

    auto getTile(int row, int column) -> Tile&;
    auto getBombCount() -> int&;

    [[nodiscard]] auto getWidth() const -> int;
    [[nodiscard]] auto getHeight() const -> int;
private:
    void create();

    void placeBombs();
    void adjustNumbers();

    auto countBombsAround(int row, int column) -> char;

    void logField();
private:
    int _width;
    int _height;
    int _bombCount;

    std::vector<Tile> _tiles;
};

#endif