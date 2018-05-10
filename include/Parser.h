// Parser - header file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#ifndef PARSER_H
#define PARSER_H

#include <map>

#include "AbstractSyntaxTree.h"
#include "Lexer.h"

#define TOK_BUF_LEN 4

namespace cparser
{

enum DeclaratorKind
{
    DK_VARIABLE,
    DK_PARAMETER,
    DK_FIELD,
    DK_TYPE_DECL,
    DK_IDENT
};

class Parser
{
protected:
    Lexer *_lex;
    SymbolTable _stb;
    AbstractSyntaxTree *_ast;

    unsigned _sym;
    int _parsingErrors;

    std::map<unsigned, const char *> _name;

    Token *_tok, *_la;
    Token _tokbuf[TOK_BUF_LEN];
    unsigned _tokIdx;
    unsigned _laIdx;
    unsigned _newLaIdx;

    void getTok();
    unsigned getLATok(unsigned k);
    const char *getLATokInfoSval(unsigned k);
    void parsingError(const char *msg);
    void check(unsigned expected);

    void initTokenBuffer();
    virtual void initNames() {}

public:
    virtual void parse(const char *output) {}

    SymbolTable *getSymbolTable()
    {
        return &_stb;
    }

    AbstractSyntaxTree *getAST()
    {
        return _ast;
    }

    unsigned getCurrentLine() const
    {
        return _tok->line;
    }

    Parser(Lexer *lex) : _lex(lex)
    {
        _parsingErrors = 0;
        _ast = new AbstractSyntaxTree(&_stb);
    }

    virtual ~Parser()
    {
        delete _ast;
    }
};

} // namespace cparser

#endif
