// C Lexer - header file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#ifndef CLEXER_H
#define CLEXER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "common.h"
#include "Lexer.h"
#include <map>

namespace cparser
{

// Token Kinds

enum TokenKind
{
    TK_UNKNOWN,
    TK_IDENT,
    TK_INT_LIT,
    TK_CHAR_LIT,
    TK_FLOAT_LIT,
    TK_STRING_LIT,

    TK_SIZEOF,

    TK_PLUS,
    TK_MINUS,
    TK_TIMES,
    TK_DIV,
    TK_MOD,

    TK_PERIODS,
    TK_SEMICOLON,
    TK_COLON,
    TK_COMMA,
    TK_PERIOD,
    TK_LPAR,
    TK_RPAR,
    TK_LBRACK,
    TK_RBRACK,
    TK_LBRACE,
    TK_RBRACE,

    TK_TILDA,   // ~
    TK_COND_OP, // ?

    TK_AND,          // &
    TK_LOGICAL_AND,  // &&
    TK_OR,           // |
    TK_LOGICAL_OR,   // ||
    TK_EXCLUSIVE_OR, // ^

    TK_PTR_OP,
    TK_INC_OP,
    TK_DEC_OP,
    TK_LSHIFT_OP,
    TK_RSHIFT_OP,

    TK_LSS,
    TK_GTR,
    TK_LEQ,
    TK_GEQ,
    TK_EQL,
    TK_NEQ,
    TK_NOT,

    TK_ASSIGN,
    TK_MUL_ASSIGN, // *=
    TK_DIV_ASSIGN,
    TK_MOD_ASSIGN,
    TK_ADD_ASSIGN,
    TK_SUB_ASSIGN,
    TK_LSHIFT_ASSIGN,
    TK_RSHIFT_ASSIGN,
    TK_AND_ASSIGN,
    TK_XOR_ASSIGN,
    TK_OR_ASSIGN,

    TK_TYPE_NAME,
    TK_ELLIPSIS, // ...

    // FirstOf Declaration
    TK_AUTO,
    TK_CHAR,
    TK_CONST,
    TK_DOUBLE,
    TK_ELSE,
    TK_ENUM,
    TK_EXTERN,
    TK_FLOAT,
    TK_INLINE, // C11
    TK_INT,
    TK_LONG,
    TK_REGISTER,
    TK_RESTRICT, // C11
    TK_SHORT,
    TK_SIGNED,
    TK_STATIC,
    TK_STRUCT,
    TK_TYPEDEF,
    TK_UNION,
    TK_UNSIGNED,
    TK_VOID,
    TK_VOLATILE,

    // FirstOf Statement
    TK_ASM,
    TK_BREAK,
    TK_CASE,
    TK_CONTINUE,
    TK_DEFAULT,
    TK_DO,
    TK_FOR,
    TK_GOTO,
    TK_IF,
    TK_RETURN,
    TK_SWITCH,
    TK_WHILE,

    // C11 keywords
    TK__ALIGNAS,
    TK__ALIGNOF,
    TK__ATOMIC,
    TK__BOOL,
    TK__COMPLEX,
    TK__GENERIC,
    TK__IMAGINARY,
    TK__NORETURN,
    TK__STATIC_ASSERT,
    TK__THREAD_LOCAL,
    TK___FUNC__,

    // GCC extension
    TK___ATTRIBUTE__,
    TK___ASM,
    TK__NULLABLE,

    TK_EOF
};

#define INSERT_KEYWORD(keyword, kind)                                          \
    _kwmap.insert(TokenMap::value_type(keyword, kind))

#define INSERT_FIRST_OF_STMT_KEYWORD(keyword, kind)                            \
    _fstmtmap.insert(TokenMap::value_type(keyword, kind))

class CLexer : public Lexer
{
    TokenMap _kwmap;
    TokenMap _fstmtmap;

    unsigned keyword(const char *s);
    void readName(Token *t);
    void readNumberLit(Token *t);
    void readStringLit(Token *t);
    void readCharLit(Token *t);
    void comment();
    void initialize();

public:
    unsigned next(Token *t);

    CLexer(const char *filename) : Lexer(filename)
    {
        initialize();
    }
};

} // namespace cparser

#endif
