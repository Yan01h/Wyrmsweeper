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

#include "mine_field.h"

#include <cassert>
#include <random>
#include <raylib.h>

MineField::MineField(int width, int height, int bombCount)
    : _width(width)
    , _height(height)
    , _bombCount(bombCount)
{
    create(bombCount);
}

auto MineField::getTile(int row, int column) -> Tile&
{
    assert(row < _height);
    assert(column < _width);
    return _tiles[column + row * _width];
}

auto MineField::getWidth() const -> int
{
    return _width;
}

auto MineField::getHeight() const -> int
{
    return _height;
}

auto MineField::getBombCount() const -> int
{
    return _bombCount;
}

void MineField::create(int bombCount)
{
    TraceLog(LOG_INFO, "Creating %ix%i mine field with %i mines", _width, _height, bombCount);
    assert(_width > 0 && _height > 0 && bombCount > 0);

    _tiles.resize(_width * _height, {0, TileState::Closed});

    placeBombs(bombCount);
    adjustNumbers();

#ifdef WS_DEBUG_BUILD
    logField();
#endif
}

void MineField::placeBombs(int bombCount)
{
    std::random_device                 randDevice;
    std::mt19937                       mersenne(randDevice());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(_tiles.size() - 1));

    for (int i = 0; i < bombCount; i++)
    {
        unsigned int bombSpot = dist(mersenne);
        while (_tiles[bombSpot].number == BOMB_NUM)
        {
            bombSpot++;
            if (bombSpot == _tiles.size())
            {
                bombSpot = 0;
            }
        }
        _tiles[bombSpot].number = BOMB_NUM;
    }
}

void MineField::adjustNumbers()
{
    for (int row = 0; row < _height; row++)
    {
        for (int column = 0; column < _width; column++)
        {
            if (getTile(row, column).number != BOMB_NUM)
            {
                getTile(row, column).number = countBombsAround(row, column);
            }
        }
    }
}

auto MineField::countBombsAround(int row, int column) -> char
{
    char bombCount = 0;
    for (int checkRow = std::max(row - 1, 0); checkRow <= std::min(row + 1, _height - 1); checkRow++)
    {
        for (int checkColumn = std::max(column - 1, 0); checkColumn <= std::min(column + 1, _width - 1); checkColumn++)
        {
            if (this->getTile(checkRow, checkColumn).number == BOMB_NUM)
            {
                bombCount++;
            }
        }
    }
    return bombCount;
}

void MineField::logField()
{
    TraceLog(LOG_INFO, "Generated field:");
    for (int row = 0; row < _height; row++)
    {
        for (int column = 0; column < _width; column++)
        {
            printf("%i ", getTile(row, column).number);
        }
        printf("\n");
    }
}