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

#ifndef WS_DEBUG_LOG_H
#define WS_DEBUG_LOG_H

#include <raylib.h>

// NOLINTBEGIN
#ifdef WS_DEBUG_BUILD
#    define LOG_INFO(message, ...) TraceLog(LOG_INFO, message, __VA_ARGS__)
#    define LOG_WARNING(message, ...) TraceLog(LOG_WARNING, message, __VA_ARGS__)
#    define LOG_ERROR(message, ...) TraceLog(LOG_ERROR, message, __VA_ARGS__)
#else
#    define LOG_INFO(message, ...)
#    define LOG_WARNING(message, ...)
#    define LOG_ERROR(message, ...)
#endif
// NOLINTEND

#endif