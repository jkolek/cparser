// Lexer - implementation file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include "../include/Lexer.h"

#include <cstdio>
#include <ctype.h>
#include <iostream>
#include <map>

namespace cparser
{

void Lexer::error(int lineNum, const char *format, ...)
{
    char dest[4096];

    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);
    printf("error: %s; line %d\n", dest, lineNum);

    exit(1);
}

bool Lexer::isHexDigit(char c)
{
    if (c >= 'A' && c <= 'F')
        return true;
    return false;
}

int Lexer::powr(int x, int n)
{
    if (n == 0)
    {
        return 1;
    }
    else if (n == 1)
    {
        return x;
    }
    else
    {
        int res = 1;

        for (int i = 0; i < n; i++)
            res = res * x;

        return res;
    }
}

void Lexer::nextCh()
{
    _ch = getc(_fp);
    _col++;
    if (_ch == '\n')
    {
        _line++;
        _col = 0;
    }
}

Lexer::Lexer(const char *filename)
{
    _line = 1;
    _col = 0;
    _ch = 0;

    // Load source file
    if (filename == NULL)
        _fp = stdin;
    else
        _fp = fopen(filename, "r");

    if (_fp == NULL)
    {
        printf("Fatal error: file '%s' does not exists!\n", filename);
        exit(1);
    }
}

} // namespace cparser
