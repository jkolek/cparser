// C Lexer - implementation file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include "../include/Lexer.h"
#include "../include/CLexer.h"

#include <cstdio>
#include <ctype.h>
#include <iostream>
#include <map>

namespace cparser
{

// Search of keyword
unsigned CLexer::keyword(const char *s)
{
    TokenMap::iterator it = _kwmap.find(s);

    if (it != _kwmap.end())
    {
        return it->second;
    }
    else
    {
        // Search of keyword that can be first in a statement
        it = _fstmtmap.find(s);
        return (it != _fstmtmap.end()) ? (it->second) : TK_IDENT;
    }
}

void CLexer::readName(Token *t)
{
    t->sval.clear();
    t->sval.push_back(_ch);
    nextCh();

    while (isalnum(_ch) || _ch == '_')
    {
        t->sval.push_back(_ch);
        nextCh();
    }

    t->kind = keyword(t->sval.c_str());
}

void CLexer::readNumberLit(Token *t)
{
    char buffer[32];
    int n = 1;
    bool hex = false, real = false;

    t->kind = TK_INT_LIT;
    buffer[0] = _ch;
    nextCh();

    if (_ch == 'x' && buffer[0] == '0')
    {
        nextCh();
        hex = true;
        while (isdigit(_ch) || isHexDigit(_ch))
        {
            buffer[n++] = _ch;
            nextCh();
        }
        // TODO: Handle this
        if (_ch == 'L')
            nextCh();
    }
    else
    {
        while (isdigit(_ch))
        {
            buffer[n++] = _ch;
            nextCh();
        }
    }

    if (_ch == '.')
    {
        real = true;
        nextCh();
    }

    if (hex)
    {
        int currentDigit, result = 0, power = 0;

        n--;
        while (n >= 0)
        {
            if (isdigit(buffer[n]))
                currentDigit = buffer[n] - '0';
            else
                currentDigit = buffer[n] - '7';

            result += currentDigit * powr(16, power);
            power++;
            n--;
        }
        t->info.ival = result;
    }
    else
    {
        int i, digitcnt = 10;

        t->info.ival = buffer[0] - '0';
        for (i = 1; i < n; i++)
            t->info.ival = t->info.ival * 10 + buffer[i] - '0';

        if (real)
        {
            int mantissa = _ch - '0';

            t->kind = TK_FLOAT_LIT;
            nextCh();
            while (isdigit(_ch))
            {
                mantissa = mantissa * 10 + _ch - '0';
                digitcnt *= 10;
                nextCh();
            }
            t->info.fval =
                ((float)t->info.ival) + ((float)mantissa / (float)digitcnt);
        }
    }
}

// TODO: Handle escape characters like: \", \n, \t, ...
void CLexer::readStringLit(Token *t)
{
    // Skip the "
    nextCh();
    t->kind = TK_STRING_LIT;
    t->sval.clear();
    t->sval.push_back(_ch);
    nextCh();

    while (_ch != '"')
    {
        t->sval.push_back(_ch);
        nextCh();
    }

    // Skip again the "
    nextCh();

    bool searchNextStringLit = true;

    do
    {
        while (isspace(_ch))
            nextCh();

        if (_ch == '"')
        {
            nextCh();
            while (_ch != '"')
            {
                t->sval.push_back(_ch);
                nextCh();
            }
            // Skip again the "
            nextCh();
        }
        else
        {
            searchNextStringLit = false;
        }
    }
    while (searchNextStringLit);
}

void CLexer::readCharLit(Token *t)
{
    // Skip the '
    nextCh();
    t->kind = TK_CHAR_LIT;

    if (_ch == '\\')
    {
        nextCh();
        switch (_ch)
        {
            case '0':
                t->info.ival = 0;
                break; // Null char
            case 'b':
                t->info.ival = 8;
                break; // Backspace
            case 't':
                t->info.ival = 9;
                break; // Horizontal tab
            case 'n':
                t->info.ival = 10;
                break; // New line
            case 'v':
                t->info.ival = 11;
                break; // Vertical tab
            case 'f':
                t->info.ival = 12;
                break; // Form feed
            case 'r':
                t->info.ival = 13;
                break; // Carriage return
            case '\'':
                t->info.ival = 39;
                break; // Single quote
            case '\\':
                t->info.ival = 92;
                break; // Backslash
            default:
                error(_line, "invalid character constant");
                break;
        }
        nextCh();
    }
    else
    {
        t->info.ival = _ch;
        nextCh();
    }

    if (_ch != '\'')
    {
        error(_line, "character expected");
        while (_ch != '\'' && _ch != EOF)
        {
            // error
            nextCh();
        }
    }

    // Skip '
    nextCh();
}

void CLexer::comment()
{
    while (_ch != EOF)
    {
        nextCh();
        if (_ch == '*')
        {
            nextCh();
            if (_ch == '/')
                break;
        }
        else if (_ch == '/')
        {
            nextCh();
            if (_ch == '*')
                comment();
        }
    }
    nextCh();
}

unsigned CLexer::next(Token *t)
{
    while (isspace(_ch))
        nextCh();

    t->line = _line;
    t->col = _col;

    if (isalpha(_ch) || _ch == '_')
    {
        readName(t);
    }
    else if (isdigit(_ch))
    {
        readNumberLit(t);
    }
    else
    {
        switch (_ch)
        {
            case '"':
                readStringLit(t);
                break;
            case '\'':
                readCharLit(t);
                break;
            case '#':
                while (_ch != '\n')
                    nextCh();
                return next(t);
            case '&':
                nextCh();
                if (_ch == '&')
                {
                    nextCh();
                    t->kind = TK_LOGICAL_AND;
                }
                else if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_AND_ASSIGN;
                }
                else
                {
                    t->kind = TK_AND;
                }
                break;
            case '!':
                nextCh();
                if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_NEQ;
                }
                else
                {
                    t->kind = TK_NOT;
                }
                break;
            case '|':
                nextCh();
                if (_ch == '|')
                {
                    nextCh();
                    t->kind = TK_LOGICAL_OR;
                }
                else if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_OR_ASSIGN;
                }
                else
                {
                    t->kind = TK_OR;
                }
                break;
            case '{':
                nextCh();
                t->kind = TK_LBRACE;
                break;
            case '}':
                nextCh();
                t->kind = TK_RBRACE;
                break;
            case '(':
                nextCh();
                t->kind = TK_LPAR;
                break;
            case ')':
                nextCh();
                t->kind = TK_RPAR;
                break;
            case '*':
                nextCh();
                if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_MUL_ASSIGN;
                }
                else
                {
                    t->kind = TK_TIMES;
                }
                break;
            case '+':
                nextCh();
                if (_ch == '+')
                {
                    nextCh();
                    t->kind = TK_INC_OP;
                }
                else if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_ADD_ASSIGN;
                }
                else
                {
                    t->kind = TK_PLUS;
                }
                break;
            case '-':
                nextCh();
                if (_ch == '-')
                {
                    nextCh();
                    t->kind = TK_DEC_OP;
                }
                else if (_ch == '>')
                {
                    nextCh();
                    t->kind = TK_PTR_OP;
                }
                else if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_SUB_ASSIGN;
                }
                else
                {
                    t->kind = TK_MINUS;
                }
                break;
            case '%':
                nextCh();
                if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_MOD_ASSIGN;
                }
                else
                {
                    t->kind = TK_MOD;
                }
                break;
            case ',':
                nextCh();
                t->kind = TK_COMMA;
                break;
            case '/':
                nextCh();
                if (_ch == '/')
                {
                    while (_ch != '\n')
                        nextCh();
                    return next(t);
                }
                else if (_ch == '*')
                {
                    comment();
                    return next(t);
                }
                else if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_DIV_ASSIGN;
                }
                else
                {
                    t->kind = TK_DIV;
                }
                break;
            case ';':
                nextCh();
                t->kind = TK_SEMICOLON;
                break;
            case '=':
                nextCh();
                if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_EQL;
                }
                else
                {
                    t->kind = TK_ASSIGN;
                }
                break;
            case '[':
                nextCh();
                t->kind = TK_LBRACK;
                break;
            case ']':
                nextCh();
                t->kind = TK_RBRACK;
                break;
            case '^':
                nextCh();
                if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_XOR_ASSIGN;
                }
                else
                {
                    t->kind = TK_EXCLUSIVE_OR;
                }
                break;
            case '~':
                nextCh();
                t->kind = TK_TILDA;
                break;
            case '?':
                nextCh();
                t->kind = TK_COND_OP;
                break;
            case ':':
                nextCh();
                t->kind = TK_COLON;
                break;
            case '<':
                nextCh();
                if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_LEQ;
                }
                else if (_ch == '<')
                {
                    nextCh();
                    if (_ch == '=')
                    {
                        nextCh();
                        t->kind = TK_LSHIFT_ASSIGN;
                    }
                    else
                    {
                        t->kind = TK_LSHIFT_OP;
                    }
                }
                else
                {
                    t->kind = TK_LSS;
                }
                break;
            case '>':
                nextCh();
                if (_ch == '=')
                {
                    nextCh();
                    t->kind = TK_GEQ;
                }
                else if (_ch == '>')
                {
                    nextCh();
                    if (_ch == '=')
                    {
                        nextCh();
                        t->kind = TK_RSHIFT_ASSIGN;
                    }
                    else
                    {
                        t->kind = TK_RSHIFT_OP;
                    }
                }
                else
                {
                    t->kind = TK_GTR;
                }
                break;
            case '.':
                nextCh();
                if (_ch == '.')
                {
                    nextCh();
                    if (_ch == '.')
                    {
                        nextCh();
                        t->kind = TK_ELLIPSIS;
                    }
                    else
                    {
                        t->kind = TK_UNKNOWN;
                    }
                }
                else
                    t->kind = TK_PERIOD;
                break;
            case EOF:
                t->kind = TK_EOF;
                break;
            default:
                nextCh();
                t->kind = TK_UNKNOWN;
                break;
        }
    }

    return t->kind;
}

void CLexer::initialize()
{
    // Insert reserved words
    INSERT_KEYWORD("_Alignas", TK__ALIGNAS);
    INSERT_KEYWORD("_Alignof", TK__ALIGNOF);
    INSERT_KEYWORD("_Atomic", TK__ATOMIC);
    INSERT_KEYWORD("_Bool", TK__BOOL);
    INSERT_KEYWORD("_Complex", TK__COMPLEX);
    INSERT_KEYWORD("_Generic", TK__GENERIC);
    INSERT_KEYWORD("_Imaginary", TK__IMAGINARY);
    INSERT_KEYWORD("_Noreturn", TK__NORETURN);
    INSERT_KEYWORD("_Nullable", TK__NULLABLE);
    INSERT_KEYWORD("_Static_assert", TK__STATIC_ASSERT);
    INSERT_KEYWORD("_Thread_local", TK__THREAD_LOCAL);
    INSERT_KEYWORD("__func__", TK___FUNC__);

    INSERT_KEYWORD("__attribute__", TK___ATTRIBUTE__);
    INSERT_KEYWORD("__asm", TK___ASM);

    INSERT_KEYWORD("auto", TK_AUTO);
    INSERT_KEYWORD("char", TK_CHAR);
    INSERT_KEYWORD("const", TK_CONST);
    INSERT_KEYWORD("double", TK_DOUBLE);
    INSERT_KEYWORD("else", TK_ELSE);
    INSERT_KEYWORD("enum", TK_ENUM);
    INSERT_KEYWORD("extern", TK_EXTERN);
    INSERT_KEYWORD("float", TK_FLOAT);
    INSERT_KEYWORD("inline", TK_INLINE);
    INSERT_KEYWORD("int", TK_INT);
    INSERT_KEYWORD("long", TK_LONG);
    INSERT_KEYWORD("register", TK_REGISTER);
    INSERT_KEYWORD("restrict", TK_RESTRICT);
    INSERT_KEYWORD("short", TK_SHORT);
    INSERT_KEYWORD("signed", TK_SIGNED);
    INSERT_KEYWORD("sizeof", TK_SIZEOF);
    INSERT_KEYWORD("static", TK_STATIC);
    INSERT_KEYWORD("struct", TK_STRUCT);
    INSERT_KEYWORD("typedef", TK_TYPEDEF);
    INSERT_KEYWORD("union", TK_UNION);
    INSERT_KEYWORD("unsigned", TK_UNSIGNED);
    INSERT_KEYWORD("void", TK_VOID);
    INSERT_KEYWORD("volatile", TK_VOLATILE);

    INSERT_FIRST_OF_STMT_KEYWORD("_Generic", TK__GENERIC);
    INSERT_FIRST_OF_STMT_KEYWORD("asm", TK_ASM);
    INSERT_FIRST_OF_STMT_KEYWORD("break", TK_BREAK);
    INSERT_FIRST_OF_STMT_KEYWORD("case", TK_CASE);
    INSERT_FIRST_OF_STMT_KEYWORD("continue", TK_CONTINUE);
    INSERT_FIRST_OF_STMT_KEYWORD("default", TK_DEFAULT);
    INSERT_FIRST_OF_STMT_KEYWORD("do", TK_DO);
    INSERT_FIRST_OF_STMT_KEYWORD("for", TK_FOR);
    INSERT_FIRST_OF_STMT_KEYWORD("goto", TK_GOTO);
    INSERT_FIRST_OF_STMT_KEYWORD("if", TK_IF);
    INSERT_FIRST_OF_STMT_KEYWORD("return", TK_RETURN);
    INSERT_FIRST_OF_STMT_KEYWORD("switch", TK_SWITCH);
    INSERT_FIRST_OF_STMT_KEYWORD("while", TK_WHILE);
}

} // namespace cparser
