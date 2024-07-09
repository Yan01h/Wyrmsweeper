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

#include <exception>

#include "core/wyrmsweeper.h"
#include "debug/log.h"

auto main(int /*argc*/, char** /*argv*/) -> int
{
    try
    {
        Wyrmsweeper game;
        game.run();
    }
    catch (const std::exception& err)
    {
        SPDLOG_CRITICAL("{}", err.what());
        return -1;
    }
    catch (...)
    {
        SPDLOG_CRITICAL("An unknown exception has occurred!");
        return -2;
    }
    return 0;
}

#if defined(WS_PLATFORM_WINDOWS) && !defined(WS_DEBUG_BUILD)
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>

int WINAPI WinMain(_In_ HINSTANCE /*hInstance*/, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPSTR /*lpCmdLine*/,
                   _In_ int /*nShowCmd*/)
{
    return main(__argc, __argv);
}

#endif