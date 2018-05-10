// Lexer - header file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "common.h"
#include <map>

namespace cparser
{

// Token type
struct Token
{
    unsigned kind;
    union
    {
        int ival;
        float fval;
    } info;
    std::string sval;
    int line, col;
};

typedef std::map<const char *, unsigned, CharCompare> TokenMap;

class Lexer
{
protected:
    FILE *_fp;
    int _ch;   // Current character
    int _line; // Current line
    int _col;  // Current column

    void error(int lineNum, const char *format, ...);
    bool isHexDigit(char c);
    int powr(int x, int n);

    virtual void readName(Token *t) {}
    virtual void readNumberLit(Token *t) {}
    virtual void readStringLit(Token *t) {}
    virtual void readCharLit(Token *t) {}
    virtual void comment() {}

public:
    void nextCh();
    virtual unsigned next(Token *t) { return 0; }

    Lexer(const char *filename);

    ~Lexer() {}
};

} // namespace cparser

#endif
