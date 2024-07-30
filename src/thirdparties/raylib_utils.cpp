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

#include "raylib_utils.h"

constexpr float FONT_BASE_SIZE = 32.F;

constexpr int FONT_TTF_DEFAULT_NUMCHARS = 95;

namespace RaylibUtils {

auto loadTextureFromMemory(const int width, const int height, const unsigned int* data) -> Texture2D
{
    Image image;
    image.width   = width;
    image.height  = height;
    image.data    = (void*)data; // NOLINT
    image.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    image.mipmaps = 1;

    return LoadTextureFromImage(image);
}

auto loadFontFromMemory(const unsigned char* data, const int size) -> Font
{
    return LoadFontFromMemory(".ttf", data, size, FONT_BASE_SIZE, nullptr, FONT_TTF_DEFAULT_NUMCHARS);
}

} // namespace RaylibUtils
