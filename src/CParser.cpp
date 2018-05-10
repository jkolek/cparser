// C parser - implementation file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include "../include/Parser.h"
#include "../include/CParser.h"
#include "../include/AbstractSyntaxTree.h"
#include "../include/Lexer.h"
#include <cassert>
#include <iostream>
#include <stack>

namespace cparser
{

// TODO: Fix the typedef

bool CParser::isTypeSpecifier(unsigned kind, int n)
{
    if (kind == TK_CHAR || kind == TK_DOUBLE || kind == TK_ENUM ||
        kind == TK_FLOAT || kind == TK_INT || kind == TK_LONG ||
        kind == TK_SHORT || kind == TK_SIGNED || kind == TK_STRUCT ||
        kind == TK_UNION || kind == TK_UNSIGNED || kind == TK_VOID)
    {
        return true;
    }
    else if (kind == TK_IDENT)
    {
        STObject *obj = _stb.find(getLATokInfoSval(n));
        if (obj != _stb.noObj && obj->kind == STOK_TYPE)
            return true;
    }

    return false;
}

bool CParser::isTypeQualifier(unsigned kind)
{
    return kind == TK_CONST || kind == TK_VOLATILE || kind == TK_RESTRICT ||
           kind == TK__ATOMIC || _sym == TK__NULLABLE;
}

bool CParser::isStorageClassSpecifier(unsigned kind)
{
    return kind == TK_TYPEDEF || kind == TK_EXTERN || kind == TK_STATIC ||
           kind == TK_AUTO || kind == TK_REGISTER;
}

//
// Initialization
//

void CParser::initNames()
{
    _name.insert(std::make_pair(TK_UNKNOWN, "unknown"));
    _name.insert(std::make_pair(TK_IDENT, "identifier"));
    _name.insert(std::make_pair(TK_INT_LIT, "integer constant"));
    _name.insert(std::make_pair(TK_CHAR_LIT, "character constant"));
    _name.insert(std::make_pair(TK_FLOAT_LIT, "float constant"));
    _name.insert(std::make_pair(TK_STRING_LIT, "string constant"));
    _name.insert(std::make_pair(TK_SIZEOF, "sizeof"));
    _name.insert(std::make_pair(TK_PLUS, "+"));
    _name.insert(std::make_pair(TK_MINUS, "-"));
    _name.insert(std::make_pair(TK_TIMES, "*"));
    _name.insert(std::make_pair(TK_DIV, "/"));
    _name.insert(std::make_pair(TK_MOD, "%"));
    _name.insert(std::make_pair(TK_PERIODS, ".."));
    _name.insert(std::make_pair(TK_SEMICOLON, ";"));
    _name.insert(std::make_pair(TK_COLON, ":"));
    _name.insert(std::make_pair(TK_COMMA, ","));
    _name.insert(std::make_pair(TK_PERIOD, "."));
    _name.insert(std::make_pair(TK_LPAR, "("));
    _name.insert(std::make_pair(TK_RPAR, ")"));
    _name.insert(std::make_pair(TK_LBRACK, "["));
    _name.insert(std::make_pair(TK_RBRACK, "]"));
    _name.insert(std::make_pair(TK_LBRACE, "{"));
    _name.insert(std::make_pair(TK_RBRACE, "}"));
    _name.insert(std::make_pair(TK_TILDA, "~"));
    _name.insert(std::make_pair(TK_COND_OP, "?"));
    _name.insert(std::make_pair(TK_AND, "&"));
    _name.insert(std::make_pair(TK_LOGICAL_AND, "&&"));
    _name.insert(std::make_pair(TK_OR, "|"));
    _name.insert(std::make_pair(TK_LOGICAL_OR, "||"));
    _name.insert(std::make_pair(TK_EXCLUSIVE_OR, "^"));
    _name.insert(std::make_pair(TK_PTR_OP, "->"));
    _name.insert(std::make_pair(TK_INC_OP, "++"));
    _name.insert(std::make_pair(TK_DEC_OP, "--"));
    _name.insert(std::make_pair(TK_LSHIFT_OP, "<<"));
    _name.insert(std::make_pair(TK_RSHIFT_OP, ">>"));
    _name.insert(std::make_pair(TK_LSS, "<"));
    _name.insert(std::make_pair(TK_GTR, ">"));
    _name.insert(std::make_pair(TK_LEQ, "<="));
    _name.insert(std::make_pair(TK_GEQ, ">="));
    _name.insert(std::make_pair(TK_EQL, "=="));
    _name.insert(std::make_pair(TK_NEQ, "!="));
    _name.insert(std::make_pair(TK_NOT, "!"));
    _name.insert(std::make_pair(TK_ASSIGN, "="));
    _name.insert(std::make_pair(TK_MUL_ASSIGN, "*="));
    _name.insert(std::make_pair(TK_DIV_ASSIGN, "/="));
    _name.insert(std::make_pair(TK_MOD_ASSIGN, "%="));
    _name.insert(std::make_pair(TK_ADD_ASSIGN, "+="));
    _name.insert(std::make_pair(TK_SUB_ASSIGN, "-="));
    _name.insert(std::make_pair(TK_LSHIFT_ASSIGN, "<<"));
    _name.insert(std::make_pair(TK_RSHIFT_ASSIGN, ">>"));
    _name.insert(std::make_pair(TK_AND_ASSIGN, "&="));
    _name.insert(std::make_pair(TK_XOR_ASSIGN, "^="));
    _name.insert(std::make_pair(TK_OR_ASSIGN, "|="));
    _name.insert(std::make_pair(TK_TYPE_NAME, "type name"));
    _name.insert(std::make_pair(TK_ELLIPSIS, "..."));
    _name.insert(std::make_pair(TK_AUTO, "auto"));
    _name.insert(std::make_pair(TK_CHAR, "char"));
    _name.insert(std::make_pair(TK_CONST, "const"));
    _name.insert(std::make_pair(TK_DOUBLE, "double"));
    _name.insert(std::make_pair(TK_ELSE, "else"));
    _name.insert(std::make_pair(TK_ENUM, "enum"));
    _name.insert(std::make_pair(TK_EXTERN, "extern"));
    _name.insert(std::make_pair(TK_FLOAT, "float"));
    _name.insert(std::make_pair(TK_INT, "int"));
    _name.insert(std::make_pair(TK_LONG, "long"));
    _name.insert(std::make_pair(TK_REGISTER, "register"));
    _name.insert(std::make_pair(TK_SHORT, "short"));
    _name.insert(std::make_pair(TK_SIGNED, "signed"));
    _name.insert(std::make_pair(TK_STATIC, "static"));
    _name.insert(std::make_pair(TK_STRUCT, "struct"));
    _name.insert(std::make_pair(TK_TYPEDEF, "typedef"));
    _name.insert(std::make_pair(TK_UNION, "union"));
    _name.insert(std::make_pair(TK_UNSIGNED, "unsigned"));
    _name.insert(std::make_pair(TK_VOID, "void"));
    _name.insert(std::make_pair(TK_VOLATILE, "volatile"));
    _name.insert(std::make_pair(TK_ASM, "asm"));
    _name.insert(std::make_pair(TK_BREAK, "break"));
    _name.insert(std::make_pair(TK_CASE, "case"));
    _name.insert(std::make_pair(TK_CONTINUE, "continue"));
    _name.insert(std::make_pair(TK_DEFAULT, "default"));
    _name.insert(std::make_pair(TK_DO, "do"));
    _name.insert(std::make_pair(TK_FOR, "for"));
    _name.insert(std::make_pair(TK_GOTO, "goto"));
    _name.insert(std::make_pair(TK_IF, "if"));
    _name.insert(std::make_pair(TK_RETURN, "return"));
    _name.insert(std::make_pair(TK_SWITCH, "switch"));
    _name.insert(std::make_pair(TK_WHILE, "while"));
    _name.insert(std::make_pair(TK_EOF, "end of file"));
}

void CParser::parse(const char *output)
{
    ASTNode *tree;
    // int errors;

    _lex->nextCh();
    initTokenBuffer();

    tree = TranslationUnit();
    _ast->setRoot(tree);

    // if (!parsingErrors)
    // {
    //     if (asmOutput)
    //         _ast->getCodeGenerator()->setAsmOutput(true);
    //     errors = _ast->emitCode(output);
    // }
    // else
    // {
    //     errors = parsingErrors;
    // }

    // if (!errors)
    // {
    //     if (!_ast->getWarnings())
    //         printf("No errors; no warnings; lines: %d\n", _tok->line);
    //     else
    //         printf("No errors; warnings: %d; lines: %d\n", _ast->getWarnings(),
    //                _tok->line);
    //     if (printAST)
    //         _ast->printTree();
    //     if (printIRET)
    //         _ast->printIRExprTree();
    //     if (!asmOutput && printCodeBuffer)
    //         _ast->getCodeGenerator()->decode();
    // }
    // else
    // {
    //     printf("Errors: %d; warnings: %d\n", errors, _ast->getWarnings());
    // }
}

//
//   T H E   P A R S E R   R U L E S
//

//
// Expressions
//

ASTNode *CParser::PrimaryExpression()
{
    ASTNode *res = NULL_AST_NODE;

    if (_sym == TK_IDENT)
    {
        getTok();
        res = new IdentASTNode(_tok->sval, _tok->line);
    }
    else if (_sym == TK_INT_LIT)
    {
        getTok();
        res = new IntegerConstASTNode(_tok->info.ival);
    }
    else if (_sym == TK_CHAR_LIT)
    {
        getTok();
        res = new CharConstASTNode(_tok->info.ival);
    }
    else if (_sym == TK_STRING_LIT)
    {
        getTok();
        res = new StringConstASTNode(_tok->sval);
    }
    else if (_sym == TK_FLOAT_LIT)
    {
        getTok();
        res = new RealConstASTNode(_tok->info.fval);
    }
    else if (_sym == TK_LPAR)
    {
        getTok();
        // Is this a function call? (* func)(x, y)
        if (_sym == TK_TIMES)
        {
            getTok();
            check(TK_IDENT);
            res = new IdentASTNode(_tok->sval, _tok->line);
        }
        else
        {
            res = Expression();
        }
        check(TK_RPAR);
    }
    else if (_sym == TK__GENERIC)
    {
        res = GenericSelection();
    }
    return res;
}

ASTNode *CParser::GenericSelection()
{
    check(TK__GENERIC);
    check(TK_LPAR);
    AssignmentExpression();
    check(TK_COMMA);
    GenericAssocList();
    check(TK_RPAR);
    return NULL_AST_NODE;
}

ASTNode *CParser::GenericAssocList()
{
    SequenceASTNode *lst = new SequenceASTNode(GenericAssociation());
    while (_sym == TK_COMMA)
    {
        getTok();
        lst->add(GenericAssociation());
    }
    return lst;
}

ASTNode *CParser::GenericAssociation()
{
    if (isTypeSpecifier(_sym, 1))
    {
        TypeName();
        check(TK_COLON);
        AssignmentExpression();
    }
    else if (_sym == TK_DEFAULT)
    {
        getTok();
        check(TK_COLON);
        AssignmentExpression();
    }
    else
    {
        parsingError("expected type specifier or default");
    }

    return NULL_AST_NODE;
}

//-----------------------------------------------------------------------------+
//  a.b.c->d->e                                                                |
//                                                                             |
//  =>                                                                         |
//       .                                                                     |
//      / \                                                                    |
//     a   .                                                                   |
//        / \                                                                  |
//       b  ->                                                                 |
//          / \                                                                |
//         c  ->                                                               |
//            / \                                                              |
//           d   e                                                             |
//-----------------------------------------------------------------------------+
ASTNode *CParser::parsePostfixExpression(ASTNode *expr)
{
    if (_sym == TK_PERIOD)
    {
        ASTNode *tmp;

        getTok();
        check(TK_IDENT);
        tmp = new IdentASTNode(_tok->sval, _tok->line);
        return new StructRefASTNode(expr, parsePostfixExpression(tmp));
    }
    else if (_sym == TK_PTR_OP)
    {
        ASTNode *tmp;

        getTok();
        check(TK_IDENT);
        tmp = new IdentASTNode(_tok->sval, _tok->line);
        return new IndirectRefASTNode(NULL_AST_NODE, expr,
                                      parsePostfixExpression(tmp));
    }
    else
    {
        return expr;
    }
}

ASTNode *CParser::PostfixExpression(ASTNode *typeName)
{
    ASTNode *index = NULL_AST_NODE;
    ASTNode *expr = PrimaryExpression();

    for (;;)
    {
        if (_sym == TK_LBRACK)
        {
            getTok();
            index = Expression();
            expr = new ArrayRefASTNode(typeName, expr, index);
            check(TK_RBRACK);
        }
        else if (_sym == TK_LPAR)
        {
            ASTNode *args;
            getTok();

            args = NULL_AST_NODE;
            if (_sym != TK_RPAR)
                args = ArgumentExpressionList();
            check(TK_RPAR);
            expr = new CallExprASTNode(expr, args);
        }
        else if (_sym == TK_PERIOD)
        {
            ASTNode *tmp;

            getTok();
            check(TK_IDENT);
            tmp = new IdentASTNode(_tok->sval, _tok->line);
            expr = new StructRefASTNode(expr, parsePostfixExpression(tmp));
        }
        else if (_sym == TK_PTR_OP)
        { // '->'
            ASTNode *tmp;

            getTok();
            check(TK_IDENT);
            tmp = new IdentASTNode(_tok->sval, _tok->line);
            expr = new IndirectRefASTNode(NULL_AST_NODE, expr,
                                          parsePostfixExpression(tmp));
        }
        else if (_sym == TK_INC_OP)
        { // '++'
            getTok();
            expr = new PostincrementExprASTNode(expr);
        }
        else if (_sym == TK_DEC_OP)
        { // '--'
            getTok();
            expr = new PostdecrementExprASTNode(expr);
        }
        else
        {
            break;
        }
    }
    return expr;
}

ASTNode *CParser::ArgumentExpressionList()
{
    SequenceASTNode *args = new SequenceASTNode(AssignmentExpression());
    while (_sym == TK_COMMA)
    {
        getTok();
        args->add(AssignmentExpression());
    }
    return args;
}

bool isFirstOfPostfixExpression(unsigned n)
{
    return (n >= TK_IDENT && n <= TK_STRING_LIT) ||
           n == TK_LPAR ||
           n == TK__GENERIC;
}

ASTNode *CParser::UnaryExpression(ASTNode *typeName)
{
    ASTNode *res = NULL_AST_NODE;
    ASTNode *expr = NULL_AST_NODE;

    if (isFirstOfPostfixExpression(_sym))
    {
        res = PostfixExpression(typeName);
    }
    else if (_sym == TK_INC_OP)
    {
        getTok();
        expr = UnaryExpression(typeName);
        res = new PreincrementExprASTNode(expr);
    }
    else if (_sym == TK_DEC_OP)
    {
        getTok();
        expr = UnaryExpression(typeName);
        res = new PredecrementExprASTNode(expr);
    }
    else if (_sym == TK_AND || _sym == TK_TIMES || _sym == TK_PLUS ||
             _sym == TK_MINUS || _sym == TK_TILDA || _sym == TK_NOT)
    {
        ASTNode *type = _ast->integerTypeASTNode;
        ASTNodeKind kind = UnaryOperator();

        if (kind == NK_INDIRECT_REF || kind == NK_ADDR_EXPR)
        {
            type = new PointerTypeASTNode(_ast->integerTypeASTNode);

            if (kind == NK_ADDR_EXPR)
                res = new AddrExprASTNode(type, CastExpression());
            else
                res = new IndirectRefASTNode(type, CastExpression(),
                                             NULL_AST_NODE);
        }
        else if (kind == NK_BIT_NOT_EXPR)
        {
            res = new BitNotExprASTNode(type, CastExpression());
        }
        else if (kind == NK_LOG_NOT_EXPR)
        {
            res = new LogNotExprASTNode(type, CastExpression());
        }
    }
    else if (_sym == TK_SIZEOF)
    {
        getTok();
        if (_sym == TK_LPAR)
        {
            getTok();
            expr = TypeName();
            // FIXME: Put this into TypeName().
            //expr = TypeSpecifier();
            if (_sym == TK_TIMES)
            {
                getTok();
                expr = new PointerTypeASTNode(expr);
            }
            check(TK_RPAR);
        }
        else
        {
            expr = UnaryExpression(NULL_AST_NODE);
        }
        res = new SizeOfExprASTNode(expr);
    }
    else if (_sym == TK__ALIGNOF)
    {
        getTok();
        check(TK_LPAR);
        expr = TypeName();
        check(TK_RPAR);
        res = new AlignOfExprASTNode(expr);
    }

    return res;
}

ASTNodeKind CParser::UnaryOperator()
{
    switch (_sym)
    {
        case TK_AND:
            getTok();
            return NK_ADDR_EXPR;
        case TK_TIMES:
            getTok();
            return NK_INDIRECT_REF;
        case TK_PLUS:
            getTok();
            return NK_UNKNOWN;
        case TK_MINUS:
            getTok();
            return NK_UNKNOWN;
        case TK_TILDA:
            getTok();
            return NK_BIT_NOT_EXPR;
        case TK_NOT:
            getTok();
            return NK_LOG_NOT_EXPR;
        default:
            return NK_UNKNOWN;
    }
}

ASTNode *CParser::CastExpression()
{
    ASTNode *typeName = NULL_AST_NODE;
    ASTNode *expr = NULL_AST_NODE;
    bool cast = false;
    bool isPtrType = false;

    if (_sym == TK_LPAR && isTypeSpecifier(getLATok(2), 2))
    {
        getTok();
        cast = true;
        typeName = TypeSpecifier();

        if (_sym == TK_TIMES)
        {
            getTok();
            isPtrType = true;
        }
        check(TK_RPAR);
    }
    expr = UnaryExpression(typeName);
    if (cast)
    {
        if (isPtrType)
            typeName = new PointerTypeASTNode(typeName);

        return new CastExprASTNode(typeName, expr);
    }
    else
    {
        return expr;
    }
}

ASTNode *CParser::MultiplicativeExpression()
{
    ASTNode *res = CastExpression();

    for (;;)
    {
        if (_sym == TK_TIMES)
        {
            getTok();
            res = AST_MULT(_ast->integerTypeASTNode, res, CastExpression());
        }
        else if (_sym == TK_DIV)
        {
            getTok();
            res = new TruncDivExprASTNode(_ast->integerTypeASTNode, res,
                                          CastExpression());
        }
        else if (_sym == TK_MOD)
        {
            getTok();
            res = new TruncModExprASTNode(_ast->integerTypeASTNode, res,
                                          CastExpression());
        }
        else
        {
            break;
        }
    }

    res->setLineNum(_tok->line);
    return res;
}

ASTNode *CParser::AdditiveExpression()
{
    ASTNode *res = MultiplicativeExpression();

    for (;;)
    {
        if (_sym == TK_PLUS)
        {
            getTok();
            res = AST_PLUS(_ast->integerTypeASTNode, res,
                           MultiplicativeExpression());
        }
        else if (_sym == TK_MINUS)
        {
            getTok();
            res = AST_MINUS(_ast->integerTypeASTNode, res,
                            MultiplicativeExpression());
        }
        else
        {
            break;
        }
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::ShiftExpression()
{
    ASTNode *res = AdditiveExpression();

    for (;;)
    {
        if (_sym == TK_LSHIFT_OP)
        {
            getTok();
            res = new LShiftExprASTNode(_ast->integerTypeASTNode, res,
                                        AdditiveExpression());
        }
        else if (_sym == TK_RSHIFT_OP)
        {
            getTok();
            res = new RShiftExprASTNode(_ast->integerTypeASTNode, res,
                                        AdditiveExpression());
        }
        else
        {
            break;
        }
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::RelationalExpression()
{
    ASTNode *res = ShiftExpression();

    for (;;)
    {
        if (_sym == TK_LSS)
        {
            getTok();
            res = new LtExprASTNode(_ast->integerTypeASTNode, res,
                                    ShiftExpression());
        }
        else if (_sym == TK_GTR)
        {
            getTok();
            res = new GtExprASTNode(_ast->integerTypeASTNode, res,
                                    ShiftExpression());
        }
        else if (_sym == TK_LEQ)
        {
            getTok();
            res = new LeExprASTNode(_ast->integerTypeASTNode, res,
                                    ShiftExpression());
        }
        else if (_sym == TK_GEQ)
        {
            getTok();
            res = new GeExprASTNode(_ast->integerTypeASTNode, res,
                                    ShiftExpression());
        }
        else
        {
            break;
        }
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::EqualityExpression()
{
    ASTNode *res = RelationalExpression();

    for (;;)
    {
        if (_sym == TK_EQL)
        {
            getTok();
            res = new EqExprASTNode(_ast->integerTypeASTNode, res,
                                    RelationalExpression());
        }
        else if (_sym == TK_NEQ)
        {
            getTok();
            res = new NeExprASTNode(_ast->integerTypeASTNode, res,
                                    RelationalExpression());
        }
        else
        {
            break;
        }
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::AndExpression()
{
    ASTNode *res = EqualityExpression();

    while (_sym == TK_AND)
    {
        getTok();
        res = new BitAndExprASTNode(_ast->integerTypeASTNode, res,
                                    EqualityExpression());
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::ExclusiveOrExpression()
{
    ASTNode *res = AndExpression();

    while (_sym == TK_EXCLUSIVE_OR)
    {
        getTok();
        res = new BitXorExprASTNode(_ast->integerTypeASTNode, res,
                                    AndExpression());
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::InclusiveOrExpression()
{
    ASTNode *res = ExclusiveOrExpression();

    while (_sym == TK_OR)
    {
        getTok();
        res = new BitIorExprASTNode(_ast->integerTypeASTNode, res,
                                    ExclusiveOrExpression());
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::LogicalAndExpression()
{
    ASTNode *res = InclusiveOrExpression();

    while (_sym == TK_LOGICAL_AND)
    {
        getTok();
        res = new LogAndExprASTNode(_ast->integerTypeASTNode, res,
                                    InclusiveOrExpression());
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::LogicalOrExpression()
{
    ASTNode *res = LogicalAndExpression();

    while (_sym == TK_LOGICAL_OR)
    {
        getTok();
        res = new LogOrExprASTNode(_ast->integerTypeASTNode, res,
                                   LogicalAndExpression());
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::ConditionalExpression()
{
    ASTNode *expr = NULL_AST_NODE;
    ASTNode *res = LogicalOrExpression();

    if (_sym == TK_COND_OP)
    {
        getTok();
        expr = Expression();
        check(TK_COLON);
        res = new CondExprASTNode(res, expr, ConditionalExpression());
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::AssignmentExpression()
{
    ASTNode *res = ConditionalExpression();

    if (_sym >= TK_ASSIGN && _sym <= TK_OR_ASSIGN)
    {
        unsigned op = _sym;

        getTok();
        if (op >= TK_MUL_ASSIGN && op <= TK_OR_ASSIGN)
        {
            ASTNode *lhs = res;
            ASTNode *rhs = AssignmentExpression();
            ASTNode *expr = NULL_AST_NODE;

            switch (op)
            {
                case TK_MUL_ASSIGN:
                    expr = AST_MULT(_ast->integerTypeASTNode, lhs, rhs);
                    break;
                case TK_DIV_ASSIGN:
                    expr = new TruncDivExprASTNode(_ast->integerTypeASTNode,
                                                   lhs, rhs);
                    break;
                case TK_MOD_ASSIGN:
                    expr = new TruncModExprASTNode(_ast->integerTypeASTNode,
                                                   lhs, rhs);
                    break;
                case TK_ADD_ASSIGN:
                    expr = AST_PLUS(_ast->integerTypeASTNode, lhs, rhs);
                    break;
                case TK_SUB_ASSIGN:
                    expr = AST_MINUS(_ast->integerTypeASTNode, lhs, rhs);
                    break;
                case TK_LSHIFT_ASSIGN:
                    expr = new LShiftExprASTNode(_ast->integerTypeASTNode, lhs,
                                                 rhs);
                    break;
                case TK_RSHIFT_ASSIGN:
                    expr = new RShiftExprASTNode(_ast->integerTypeASTNode, lhs,
                                                 rhs);
                    break;
                case TK_AND_ASSIGN:
                    expr = new BitAndExprASTNode(_ast->integerTypeASTNode, lhs,
                                                 rhs);
                    break;
                case TK_XOR_ASSIGN:
                    expr = new BitXorExprASTNode(_ast->integerTypeASTNode, lhs,
                                                 rhs);
                    break;
                case TK_OR_ASSIGN:
                    expr = new BitIorExprASTNode(_ast->integerTypeASTNode, lhs,
                                                 rhs);
                    break;
            }
            res = new AssignExprASTNode(_ast->integerTypeASTNode, res, expr);
        }
        else
        {
            res = new AssignExprASTNode(_ast->integerTypeASTNode, res,
                                        AssignmentExpression());
        }
    }
    res->setLineNum(_tok->line);

    return res;
}

ASTNode *CParser::Expression()
{
    ASTNode *expr = AssignmentExpression();

    while (_sym == TK_COMMA)
    {
        getTok();
        AssignmentExpression();
    }

    return expr;
}

ASTNode *CParser::ConstantExpression() { return ConditionalExpression(); }

//
// Declarations
//

ASTNode *CParser::Declaration(ASTNode *declSpec)
{
    ASTNode *initlist = NULL_AST_NODE;

    if (_sym != TK_SEMICOLON)
        initlist = InitDeclaratorList(declSpec);

    check(TK_SEMICOLON);

    if (initlist != NULL_AST_NODE)
    {
        // initlist->transform(NK_VAR_DECL, declspec);
        return initlist;
    }
    else
    {
        // Here is a forward struct declaration so far.
        return declSpec;
    }
}

ASTNode *CParser::DeclarationSpecifiers()
{
    ASTNode *declSpec = NULL_AST_NODE;
    // ASTNodeKind storageClassSpec = NK_UNKNOWN;
    unsigned flags = 0x0;

    for (;;)
    {
        if (isStorageClassSpecifier(_sym))
        {
            StorageClassSpecifier(flags);
        }
        else if (isTypeQualifier(_sym))
        {
            TypeQualifier(flags);
        }
        else if (isTypeSpecifier(_sym, 1))
        {
            declSpec = TypeSpecifier();
            declSpec->setFlags(flags);
        }
        // else if (isTypeSpecifier(_sym, 1))
        // {
        //     TypeSpecifier();
        // }
        else if (_sym == TK_TIMES) // ??? TODO: Check grammar ???
        {
            getTok();
            declSpec = new PointerTypeASTNode(declSpec);
            if (isTypeQualifier(_sym))
            {
                TypeQualifier(flags);
            }
        }
        else if (_sym == TK___ASM || _sym == TK___ATTRIBUTE__)
        {
            GccDeclaratorExtension();
        }
        else if (_sym == TK_INLINE || _sym == TK__NORETURN)
        {
            // TODO: FunctionSpecifier()
            getTok();
        }
        else
        {
            break;
        }
    }

    return declSpec;
}

SequenceASTNode *CParser::InitDeclaratorList(ASTNode *typeSpec)
{
    SequenceASTNode *initlist = new SequenceASTNode(InitDeclarator(typeSpec));

    while (_sym == TK_COMMA)
    {
        getTok();
        initlist->add(InitDeclarator(typeSpec));
    }

    return initlist;
}

ASTNode *CParser::InitDeclarator(ASTNode *typeSpec)
{
    ASTNode *init = NULL_AST_NODE;
    VarDeclASTNode *declr = new VarDeclASTNode(typeSpec, Declarator(typeSpec));

    if (_sym == TK_ASSIGN)
    {
        getTok();
        init = Initializer();
        // if (declr->getKind() == NK_VAR_DECL)
        // {
        //     VarDeclASTNode *varDecl = static_cast<VarDeclASTNode *>(declr);
        //     varDecl->setInit(init);
        // }
        declr->setInit(init);
    }

    return declr;
}

void CParser::StorageClassSpecifier(unsigned &flags)
{
    if (_sym == TK_TYPEDEF)
    {
        getTok();
        //flags |= SCS_TYPEDEF;
        _inTypedef = true;
    }
    else if (_sym == TK_EXTERN)
    {
        getTok();
        flags |= SCS_EXTERN;
    }
    else if (_sym == TK_STATIC)
    {
        getTok();
        flags |= SCS_STATIC;
    }
    else if (_sym == TK__THREAD_LOCAL)
    {
        getTok();
    }
    else if (_sym == TK_AUTO)
    {
        getTok();
        flags |= SCS_AUTO;
    }
    else if (_sym == TK_REGISTER)
    {
        getTok();
        flags |= SCS_REGISTER;
    }
}

static ASTNode *
stbTypeToASTNodeType(AbstractSyntaxTree *ast, STType *type, const char *objName)
{
    switch (type->kind)
    {
        case STTK_NONE:
            return NULL_AST_NODE;
        case STTK_CHAR:
            return ast->charTypeASTNode;
        case STTK_SHORT:
            return ast->shortTypeASTNode;
        case STTK_INT:
            return ast->integerTypeASTNode;
        case STTK_UNSIGNED:
            return ast->unsignedTypeASTNode;
        case STTK_LONG:
            return ast->longTypeASTNode;
            //return NULL_AST_NODE;
        case STTK_FLOAT:
            return ast->floatTypeASTNode;
        case STTK_DOUBLE:
            // return IR_f64;
            return NULL_AST_NODE;
        case STTK_VOID:
            return ast->voidTypeASTNode;
        case STTK_ARRAY:
            //  return IR_i32;
            return NULL_AST_NODE;
        case STTK_STRUCT:
            // return _ast->createStructTypeASTNode(new IdentASTNode(objName),
            //                                    NULL_AST_NODE);
            return new StructTypeASTNode(new IdentASTNode(objName),
                                         NULL_AST_NODE);
        case STTK_UNION:
            return NULL_AST_NODE;
        case STTK_BOOL:
            return NULL_AST_NODE;
        case STTK_REAL:
            return NULL_AST_NODE;
        case STTK_POINTER:
            return new PointerTypeASTNode(stbTypeToASTNodeType(ast,
                                                               type->baseType,
                                                               objName));
        case STTK_ENUM:
            return NULL_AST_NODE;
        case STTK_FUNCTION:
            return NULL_AST_NODE;
    }
    return NULL_AST_NODE;
}

ASTNode *CParser::TypeSpecifier()
{
    if (_sym == TK_VOID)
    {
        getTok();
        return _ast->voidTypeASTNode;
    }
    else if (_sym == TK_CHAR)
    {
        getTok();
        return _ast->charTypeASTNode;
    }
    else if (_sym == TK_SHORT)
    {
        getTok();
        return _ast->shortTypeASTNode;
    }
    else if (_sym == TK_INT)
    {
        getTok();
        return _ast->integerTypeASTNode;
    }
    else if (_sym == TK_LONG)
    {
        getTok();
        return _ast->longTypeASTNode;
    }
    else if (_sym == TK_FLOAT)
    {
        getTok();
        return _ast->floatTypeASTNode;
    }
    else if (_sym == TK_DOUBLE)
    {
        getTok();
    }
    else if (_sym == TK_SIGNED)
    {
        getTok();
    }
    else if (_sym == TK_UNSIGNED)
    {
        getTok();
        return _ast->unsignedTypeASTNode;
    }
    else if (_sym == TK_STRUCT || _sym == TK_UNION)
    {
        return StructOrUnionSpecifier();
    }
    else if (_sym == TK_ENUM)
    {
        return EnumSpecifier();
    }
    else if (_sym == TK_IDENT)
    { // TYPE_NAME
        char msg[MAXSTR];
        STObject *obj;

        getTok();

        obj = _stb.find(_tok->sval.c_str());
        if (obj != _stb.noObj)
            return stbTypeToASTNodeType(_ast, obj->type, obj->name);

        sprintf(msg, "unknown type '%s'", obj->name);
        parsingError(msg);
    }

    return NULL_AST_NODE;
}

// TODO: Make distinct productions for a struct type declaration and for
//       declaration of variable of a struct type.
ASTNode *CParser::StructOrUnionSpecifier()
{
    ASTNode *typeName = NULL_AST_NODE;
    ASTNode *typeBody = NULL_AST_NODE;
    STObject *obj = nullptr;
    ASTNodeKind typeKind;

    if (_sym == TK_STRUCT)
    {
        getTok();
        typeKind = NK_STRUCT_TYPE;
    }
    else
    { // _sym == UNION
        getTok();
        typeKind = NK_UNION_TYPE;
    }

    if (_sym == TK_IDENT)
    {
        STType *recordType;

        getTok();
        typeName = new IdentASTNode(_tok->sval, _tok->line);
        if (typeKind == NK_STRUCT_TYPE)
            recordType = _stb.allocType(STTK_STRUCT);
        else
            recordType = _stb.allocType(STTK_UNION);
        obj = _stb.find(_tok->sval.c_str());
        // if (obj != _stb.noObj && obj->type->fields != NULL)
        // {
        //     _ast->error("struct type '%s' has already been declared",
        //                _tok->sval,
        //                _tok->line);
        // }
        // else
        if (obj == _stb.noObj)
            obj = _stb.insert(_tok->sval.c_str(), STOK_TYPE, recordType);
    }
    if (_sym == TK_LBRACE)
    {
        getTok();
        _stb.openScope();
        typeBody = StructDeclarationList();
        // FIXME: What if we have anonymous struct type?
        if (obj != nullptr)
        {
            if (typeKind == NK_UNION_TYPE)
            {
                STObject *tmp = _stb.getTopScope()->locals;
                unsigned maxFieldTypeSize = 0;

                while (tmp != nullptr)
                {
                    if (maxFieldTypeSize < tmp->type->size)
                        maxFieldTypeSize = tmp->type->size;
                    tmp = tmp->next;
                }
                _stb.getTopScope()->size = maxFieldTypeSize;
            }
            obj->type->fields = _stb.getTopScope()->locals;
            obj->type->size = _stb.getTopScope()->size;
        }
        _stb.closeScope();
        check(TK_RBRACE);
    }
    if (typeKind == NK_STRUCT_TYPE)
        // return _ast->createStructTypeASTNode(typeName, typeBody);
        return new StructTypeASTNode(typeName, typeBody);
    else
        return new UnionTypeASTNode(typeName, typeBody);
}

SequenceASTNode *CParser::StructDeclarationList()
{
    SequenceASTNode *declist = new SequenceASTNode(StructDeclaration());

    while (_sym != TK_RBRACE)
        declist->add(StructDeclaration());

    return declist;
}

ASTNode *CParser::StructDeclaration()
{
    //ASTNode *typeSpec = TypeSpecifier();
    ASTNode *typeSpec = SpecifierQualifierList();
    SequenceASTNode *declaratorList = StructDeclaratorList(typeSpec);

    // declaratorList->declare(_ast);
    check(TK_SEMICOLON);

    return declaratorList;
}

ASTNode *CParser::SpecifierQualifierList()
{
    SequenceASTNode *lst = new SequenceASTNode(TypeSpecifier());

    while (isTypeSpecifier(_sym, 1))
        lst->add(TypeSpecifier());

    return lst;
}

SequenceASTNode *CParser::StructDeclaratorList(ASTNode *typeSpec)
{
    SequenceASTNode *declist = new SequenceASTNode(StructDeclarator(typeSpec));

    while (_sym == TK_COMMA)
    {
        getTok();
        declist->add(StructDeclarator(typeSpec));
    }

    return declist;
}

ASTNode *CParser::StructDeclarator(ASTNode *typeSpec)
{
    return new FieldDeclASTNode(typeSpec, Declarator(typeSpec));
}

ASTNode *CParser::EnumSpecifier()
{
    ASTNode *name = NULL_AST_NODE;
    ASTNode *body = NULL_AST_NODE;

    getTok(); // Eat 'enum'.

    if (_sym == TK_IDENT)
    {
        STType *enumType;

        getTok();
        name = new IdentASTNode(_tok->sval);
        enumType = _stb.allocType(STTK_ENUM);
        _stb.insert(_tok->sval.c_str(), STOK_TYPE, enumType);
    }

    if (_sym == TK_LBRACE)
    {
        getTok();
        body = EnumeratorList();
        check(TK_RBRACE);
    }

    return new EnumeralTypeASTNode(name, body);
}

SequenceASTNode *CParser::EnumeratorList()
{
    SequenceASTNode *enums;
    STObject *obj;
    unsigned i = 0;

    check(TK_IDENT);
    enums = new SequenceASTNode(new IdentASTNode(_tok->sval));

    obj = _stb.insert(_tok->sval.c_str(), STOK_CON, _stb.intType);
    obj->ival = i;

    if (_sym == TK_ASSIGN)
    {
        getTok();
        ConstantExpression();
    }

    while (_sym == TK_COMMA)
    {
        getTok();
        check(TK_IDENT);
        enums->add(new IdentASTNode(_tok->sval));

        i++;
        obj = _stb.insert(_tok->sval.c_str(), STOK_CON, _stb.intType);
        obj->ival = i;

        if (_sym == TK_ASSIGN)
        {
            getTok();
            ConstantExpression();
        }
    }

    return enums;
}

void CParser::TypeQualifier(unsigned &flags)
{
    if (_sym == TK_CONST)
    {
        getTok();
        flags |= TQ_CONST;
    }
    else if (_sym == TK_RESTRICT)
    {
        getTok();
    }
    else if (_sym == TK_VOLATILE)
    {
        getTok();
        flags |= TQ_VOLATILE;
    }
    else if (_sym == TK__ATOMIC)
    {
        getTok();
    }
    else if (_sym == TK__NULLABLE)
    {
        getTok();
    }
    else
    {
        parsingError("expected type qualifier");
    }
}

void CParser::FunctionSpecifier()
{
    if (_sym == TK_INLINE)
    {
        getTok();
    }
    else if (_sym == TK__NORETURN)
    {
        getTok();
    }
    else
    {
        parsingError("expected function specifier");
    }
}

// alignment_specifier
//     : ALIGNAS '(' type_name ')'
//     | ALIGNAS '(' constant_expression ')'
//     ;
ASTNode *CParser::AlignmentSpecifier()
{
    check(TK__ALIGNAS);
    check(TK_LPAR);
    // TODO: This is incorrect, use isFirstOfTypeName() instead.
    if (isTypeSpecifier(_sym, 1))
    {
        TypeName();
    }
    else
    {
        ConstantExpression();
    }
    check(TK_RPAR);

    return NULL_AST_NODE;
}

ASTNode *CParser::Declarator(ASTNode * &typeSpec)
{
    while (_sym == TK_TIMES)
    {
        unsigned flags;

        getTok();

        // Flags needs to be preserved
        flags = typeSpec->getFlags();
        typeSpec = new PointerTypeASTNode(typeSpec);
        typeSpec->setFlags(flags);
    }

    ASTNode *decl = DirectDeclarator(typeSpec);

    while (_sym == TK___ASM || _sym == TK___ATTRIBUTE__)
        GccDeclaratorExtension();

    return decl;
}

ASTNode *CParser::DirectDeclarator(ASTNode * &typeSpec)
{
    ASTNode *declr = NULL_AST_NODE;
    ASTNode *expr = NULL_AST_NODE;
    FunctionTypeASTNode *funcType;
    bool isFuncPtr = false;

    if (_sym == TK_IDENT)
    {
        getTok();
        declr = new IdentASTNode(_tok->sval, _tok->line);
    }
    else
    {
        check(TK_LPAR);
        if (_sym == TK_TIMES)
        {
            getTok();

            unsigned flags = 0;
            // TODO: Check the grammar
            if (isTypeQualifier(_sym))
            {
                TypeQualifier(flags);
            }

            // This is a function pointer.
            isFuncPtr = true;
            // FIXME: Make this direct type without T_POINTER.
            funcType = new FunctionTypeASTNode(typeSpec, NULL_AST_NODE);
            typeSpec = new PointerTypeASTNode(funcType);
        }
        ASTNode *dummy = NULL_AST_NODE;
        declr = Declarator(dummy);
        check(TK_RPAR);
    }
    for (;;)
    {
        if (_sym == TK_LBRACK)
        {
            getTok();

            if (_sym != TK_RBRACK)
            {
                expr = Expression();
                typeSpec = new ArrayTypeASTNode(typeSpec, expr);
            }
            else
            {
                // Declarations like int a[] are treated as a pointers.
                typeSpec = new PointerTypeASTNode(typeSpec);
            }
            check(TK_RBRACK);
        }
        // else if (_sym == TK_LPAR)
        // {
        //     getTok();
        //     if (_sym != TK_RPAR)
        //     {
        //         //
        //     }
        //     check(TK_RPAR);
        // }
        else
        {
            break;
        }
    }
    if (_sym == TK_LPAR && isFuncPtr)
    {
        getTok();
        if (_sym != TK_RPAR)
        {
            ASTNode *funcPrms = ParameterTypeList();
            funcType->setPrms(funcPrms);
        }
        check(TK_RPAR);
    }

    return declr;
}

// gccDeclaratorExtension
//     :   '__asm' '(' StringLiteral+ ')'
//     |   gccAttributeSpecifier
//     ;
ASTNode *CParser::GccDeclaratorExtension()
{
    if (_sym == TK___ASM)
    {
        getTok();
        check(TK_LPAR);
        check(TK_STRING_LIT);
        while (_sym == TK_STRING_LIT)
        {
            getTok();
        }
        check(TK_RPAR);
    }
    else if (_sym == TK___ATTRIBUTE__)
    {
        GccAttributeSpecifier();
    }
    else
    {
        parsingError("expected '__asm' or '__attribute__'");
    }

    return NULL_AST_NODE;
}

// gccAttributeSpecifier
//     :   '__attribute__' '(' '(' gccAttributeList ')' ')'
//     ;
ASTNode *CParser::GccAttributeSpecifier()
{
    check(TK___ATTRIBUTE__);
    check(TK_LPAR);
    check(TK_LPAR);
    GccAttributeList();
    check(TK_RPAR);
    check(TK_RPAR);

    return NULL_AST_NODE;
}

// gccAttributeList
//     :   gccAttribute (',' gccAttribute)*
//     |   // empty
//     ;
ASTNode *CParser::GccAttributeList()
{
    if (_sym == TK_IDENT)
    {
        SequenceASTNode *lst = new SequenceASTNode(GccAttribute());

        while (_sym == TK_COMMA)
        {
            getTok();
            lst->add(GccAttribute());
        }
        return lst;
    }
    else
    {
        return NULL_AST_NODE;
    }
}

// gccAttribute
//     :   ~(',' | '(' | ')') // relaxed def for "identifier or reserved word"
//         ('(' argumentExpressionList? ')')?
//     |   // empty
//     ;
ASTNode *CParser::GccAttribute()
{
    if (_sym == TK_IDENT)
    {
        getTok();
        if (_sym == TK_LPAR)
        {
            getTok();
            if (_sym != TK_RPAR)
            {
                ArgumentExpressionList();
            }
            check(TK_RPAR);
        }
    }

    return NULL_AST_NODE;
}

ASTNode *CParser::ParameterTypeList()
{
    ASTNode *paramList = ParameterList();
    if (_sym == TK_COMMA)
    {
        getTok();
        check(TK_ELLIPSIS);
    }
    return paramList;
}

ASTNode *CParser::ParameterList()
{
    SequenceASTNode *prms = new SequenceASTNode(ParameterDeclaration());

    while (_sym == TK_COMMA)
    {
        if (getLATok(2) == TK_ELLIPSIS)
            break;

        getTok();
        prms->add(ParameterDeclaration());
    }
    return prms;
}

static bool isFirstOfAbstractDeclarator(unsigned n)
{
    return n == TK_TIMES || n == TK_LPAR || n == TK_LBRACK;
}

// parameter_declaration
//     : declaration_specifiers declarator
//     | declaration_specifiers abstract_declarator
//     | declaration_specifiers
//     ;
ASTNode *CParser::ParameterDeclaration()
{
    ASTNode *typeSpec = DeclarationSpecifiers();
    ASTNode *declr = NULL_AST_NODE;

    if (_sym == TK_IDENT)
    {
        ASTNode *d = Declarator(typeSpec);
        declr = new ParmDeclASTNode(typeSpec, d);
    }
    else if (isFirstOfAbstractDeclarator(_sym))
    {
        declr = new ParmDeclASTNode(typeSpec,  AbstractDeclarator());
    }

    return declr;
}

ASTNode *CParser::IdentifierList()
{
    SequenceASTNode *res;

    check(TK_IDENT);
    res = new SequenceASTNode(new IdentASTNode(_tok->sval, _tok->line));

    while (_sym == TK_COMMA)
    {
        getTok();
        check(TK_IDENT);
        res->add(new IdentASTNode(_tok->sval, _tok->line));
    }

    return res;
}

// static bool isFirstOfTypeName(unsigned n)
// {
//     return false;
// }

// type_name
//     : specifier_qualifier_list abstract_declarator
//     | specifier_qualifier_list
//     ;
ASTNode *CParser::TypeName()
{
    // check(TK_IDENT);
    // return new IdentASTNode(_tok->sval, _tok->line);
    ASTNode *specQualList = SpecifierQualifierList();
    //AbstractDeclarator();
    return specQualList;
}

ASTNode *CParser::AbstractDeclarator()
{
    if (_sym == TK_TIMES)
    {
        getTok();
        if (_sym == TK_LPAR || _sym == TK_LBRACK)
            DirectAbstractDeclarator();
    }
    else if (_sym == TK_LPAR || _sym == TK_LBRACK)
    {
        DirectAbstractDeclarator();
    }
    else
    {
        parsingError("expected '*' or direct abstract declarator");
    }

    return NULL_AST_NODE;
}

ASTNode *CParser::DirectAbstractDeclarator()
{
    if (_sym == TK_LPAR)
    {
        getTok();
        if (isFirstOfAbstractDeclarator(_sym))
            AbstractDeclarator();
        check(TK_RPAR);
    }
    else if (_sym == TK_LBRACK)
    {
        getTok();
        check(TK_RBRACK);
    }

    return NULL_AST_NODE;
}

ASTNode *CParser::Initializer()
{
    ASTNode *res = NULL_AST_NODE;

    if (_sym == TK_LBRACE)
    {
        getTok();

        res = InitializerList();

        check(TK_RBRACE);
    }
    else
    {
        res = AssignmentExpression();
    }

    return res;
}

ASTNode *CParser::InitializerList()
{
    SequenceASTNode *res;
    ASTNode *init;

    init = Initializer();
    res = new SequenceASTNode(init);

    while (_sym == TK_COMMA)
    {
        getTok();

        init = Initializer();
        res->add(init);
    }

    return res;
}

//
// Statements
//

ASTNode *CParser::Statement()
{
    ASTNode *stmt = NULL_AST_NODE;
    // ASTNode *expr = NULL_AST_NODE;

    if (_sym == TK_IDENT)
    {
        if (getLATok(2) == TK_COLON)
            stmt = LabeledStatement();
        else
            stmt = ExpressionStatement();
    }
    else if (_sym == TK_TIMES || (_sym == TK_LPAR && getLATok(2) == TK_TIMES))
    {
        // This is a function call of type: (*func)(x, y)
        stmt = ExpressionStatement();
    }
    else if (_sym == TK_CASE || _sym == TK_DEFAULT)
    {
        stmt = LabeledStatement();
    }
    else if (_sym == TK_LBRACE)
    {
        stmt = CompoundStatement();
    }
    else if (_sym == TK_IF || _sym == TK_SWITCH)
    {
        stmt = SelectionStatement();
    }
    else if (_sym == TK_WHILE || _sym == TK_DO || _sym == TK_FOR)
    {
        stmt = IterationStatement();
    }
    else if (_sym == TK_GOTO || _sym == TK_CONTINUE || _sym == TK_BREAK ||
             _sym == TK_RETURN)
    {
        stmt = JumpStatement();
    }
    else if (_sym == TK_ASM)
    {
        getTok();
        check(TK_LPAR);
        check(TK_STRING_LIT);
        stmt = new AsmStmtASTNode(_tok->sval);
        if (_sym == TK_COLON)
        {
            // Output operands
            getTok();
            if (_sym != TK_COLON)
            {
                check(TK_STRING_LIT);
                check(TK_LPAR);
                Expression();
                check(TK_RPAR);
                while (_sym == TK_COMMA)
                {
                    getTok();
                    check(TK_STRING_LIT);
                    check(TK_LPAR);
                    Expression();
                    check(TK_RPAR);
                }
            }
        }
        if (_sym == TK_COLON)
        {
            // Input operands
            getTok();
            if (_sym != TK_COLON)
            {
                check(TK_STRING_LIT);
                check(TK_LPAR);
                Expression();
                check(TK_RPAR);
                while (_sym == TK_COMMA)
                {
                    getTok();
                    check(TK_STRING_LIT);
                    check(TK_LPAR);
                    Expression();
                    check(TK_RPAR);
                }
            }
        }
        if (_sym == TK_COLON)
        {
            // Clobbered inputs
            getTok();
            if (_sym != TK_RPAR)
            {
                check(TK_STRING_LIT);
                while (_sym == TK_COMMA)
                {
                    getTok();
                    check(TK_STRING_LIT);
                }
            }
        }
        check(TK_RPAR);
        check(TK_SEMICOLON);
    }

    return stmt;
}

ASTNode *CParser::LabeledStatement()
{
    ASTNode *labstmt = NULL_AST_NODE;

    if (_sym == TK_IDENT)
    {
        getTok();
        ASTNode *label = new IdentASTNode(_tok->sval, _tok->line);
        check(TK_COLON);
        ASTNode *stmt = Statement();
        labstmt = new LabelStmtASTNode(label, stmt);
    }
    else if (_sym == TK_CASE)
    {
        getTok();
        ASTNode *expr = ConstantExpression();
        check(TK_COLON);
        ASTNode *stmt = Statement();
        labstmt = new CaseLabelASTNode(expr, stmt);
    }
    else if (_sym == TK_DEFAULT)
    {
        getTok();
        check(TK_COLON);
        Statement();
    }

    return labstmt;
}

ASTNode *CParser::CompoundStatement()
{
    ASTNode *compstmt = NULL_AST_NODE;

    check(TK_LBRACE);
    if (_sym != TK_RBRACE)
        compstmt = StmtOrDeclList();
    check(TK_RBRACE);

    return compstmt;
}

ASTNode *CParser::FunctionBody()
{
    if (_sym != TK_LBRACE)
    { // Forward declaration
        return NULL_AST_NODE;
    }

    ASTNode *decl = NULL_AST_NODE;
    ASTNode *stmt = NULL_AST_NODE;
    ASTNode *declList = NULL_AST_NODE;
    ASTNode *stmtList = NULL_AST_NODE;
    ASTNode *compound = NULL_AST_NODE;

    getTok(); // eat '{'
    compound = new CompoundStmtASTNode(NULL_AST_NODE, NULL_AST_NODE);
    if (_sym != TK_RBRACE)
    {
        // The scope has been opened in FunctionDeclASTNode.

        // Declarations
        while (isStorageClassSpecifier(_sym) || isTypeQualifier(_sym) ||
               isTypeSpecifier(_sym, 1))
        {
            ASTNode *declSpec = DeclarationSpecifiers();

            if (_inTypedef)
            {
                _inTypedef = false;
                decl = new TypeDeclASTNode(Declarator(declSpec), declSpec);
                decl->declare(_ast);
                check(TK_SEMICOLON);
            }
            else
            {
                decl = Declaration(declSpec);
            }
            //_ast->declare(decl);
            if (declList == NULL_AST_NODE)
                declList = new SequenceASTNode(decl);
            else
                static_cast<SequenceASTNode *>(declList)->add(decl);

            //_inTypedef = false;
        }

        // Statements
        for (;;)
        {
            if (_sym == TK_IDENT || _sym == TK_TIMES ||
                (_sym == TK_LPAR && getLATok(2) == TK_TIMES))
            {
                stmt = Statement();
                if (stmtList == NULL_AST_NODE)
                    stmtList = new SequenceASTNode(stmt);
                else
                    static_cast<SequenceASTNode *>(stmtList)->add(stmt);
            }
            else if (_sym >= TK_ASM && _sym <= TK_WHILE)
            {
                stmt = Statement();
                if (stmtList == NULL_AST_NODE)
                    stmtList = new SequenceASTNode(stmt);
                else
                    static_cast<SequenceASTNode *>(stmtList)->add(stmt);
            }
            else
            {
                break;
            }
        }
        static_cast<CompoundStmtASTNode *>(compound)->setDecls(declList);
        static_cast<CompoundStmtASTNode *>(compound)->setStmts(stmtList);
    }
    else
    {
        static_cast<CompoundStmtASTNode *>(compound)->setStmts(
            new SequenceASTNode(new NopExprASTNode()));
    }
    check(TK_RBRACE);

    return compound;
}

ASTNode *CParser::DeclarationList()
{
    // Declaration()
    return NULL_AST_NODE;
}

ASTNode *CParser::StmtOrDeclList()
{
    ASTNode *decl = NULL_AST_NODE;
    ASTNode *stmt = NULL_AST_NODE;
    ASTNode *stmts = NULL_AST_NODE;
    ASTNode *decls = NULL_AST_NODE;

    // Declarations
    while (isStorageClassSpecifier(_sym) || isTypeQualifier(_sym) ||
           isTypeSpecifier(_sym, 1))
    {
        // _inTypedef is set in DeclartionSpecifiers/StorageClassSpecifier
        ASTNode *declSpec = DeclarationSpecifiers();

        if (_inTypedef)
        {
            _inTypedef = false;
            decl = new TypeDeclASTNode(Declarator(declSpec), declSpec);
            decl->declare(_ast);
            check(TK_SEMICOLON);
        }
        else
        {
            decl = Declaration(declSpec);
        }
        //_ast->declare(decl);
        if (decls == NULL_AST_NODE)
            decls = new SequenceASTNode(decl);
        else
            static_cast<SequenceASTNode *>(decls)->add(decl);

        //_inTypedef = false;
    }

    // Statements
    for (;;)
    {
        if (_sym == TK_IDENT)
        {
            stmt = Statement();
            if (stmts == NULL_AST_NODE)
                stmts = new SequenceASTNode(stmt);
            else
                static_cast<SequenceASTNode *>(stmts)->add(stmt);
        }
        else if (_sym >= TK_ASM && _sym <= TK_WHILE)
        {
            stmt = Statement();
            if (stmts == NULL_AST_NODE)
                stmts = new SequenceASTNode(stmt);
            else
                static_cast<SequenceASTNode *>(stmts)->add(stmt);
        }
        else
        {
            break;
        }
    }

    return new CompoundStmtASTNode(decls, stmts);
}

ASTNode *CParser::SelectionStatement()
{
    ASTNode *selstmt = NULL_AST_NODE;

    if (_sym == TK_IF)
    {
        ASTNode *expr;
        ASTNode *then_clause;
        ASTNode *else_clause = NULL_AST_NODE;

        getTok();
        check(TK_LPAR);
        expr = Expression();
        check(TK_RPAR);
        then_clause = Statement();

        if (_sym == TK_ELSE)
        {
            getTok();
            else_clause = Statement();
        }

        selstmt = new IfStmtASTNode(expr, then_clause, else_clause);
    }
    else if (_sym == TK_SWITCH)
    {
        ASTNode *expr;
        ASTNode *stmt;

        getTok();
        check(TK_LPAR);
        expr = Expression();
        check(TK_RPAR);
        stmt = Statement();

        selstmt = new SwitchStmtASTNode(expr, stmt);
    }

    return selstmt;
}

ASTNode *CParser::IterationStatement()
{
    ASTNode *iterstmt = NULL_AST_NODE;
    ASTNode *expr = NULL_AST_NODE;
    ASTNode *stmt = NULL_AST_NODE;

    if (_sym == TK_WHILE)
    {
        getTok();
        check(TK_LPAR);

        // Get condition
        expr = Expression();
        check(TK_RPAR);

        // Get while body
        stmt = Statement();

        // Create while statement
        iterstmt = new WhileStmtASTNode(expr, stmt);
    }
    else if (_sym == TK_DO)
    {
        getTok();

        // Get do-while body
        stmt = Statement();
        check(TK_WHILE);
        check(TK_LPAR);

        // Get condition
        expr = Expression();
        check(TK_RPAR);
        check(TK_SEMICOLON);

        // Create while statement
        iterstmt = new DoStmtASTNode(expr, stmt);
    }
    else if (_sym == TK_FOR)
    {
        ASTNode *step = NULL_AST_NODE;
        ASTNode *init = NULL_AST_NODE;

        getTok();
        check(TK_LPAR);

        if (_sym != TK_SEMICOLON)
            init = Expression();

        check(TK_SEMICOLON);

        if (_sym != TK_SEMICOLON)
            expr = Expression();

        check(TK_SEMICOLON);

        if (_sym != TK_RPAR)
            step = Expression();

        check(TK_RPAR);

        stmt = Statement();

        // Create for statement
        iterstmt = new ForStmtASTNode(init, expr, step, stmt);
    }

    return iterstmt;
}

ASTNode *CParser::JumpStatement()
{
    ASTNode *jumpstmt = NULL_AST_NODE;
    ASTNode *expr = NULL_AST_NODE;

    if (_sym == TK_GOTO)
    {
        getTok();
        check(TK_IDENT);
        jumpstmt =
            new GotoStmtASTNode(new IdentASTNode(_tok->sval, _tok->line));
        check(TK_SEMICOLON);
    }
    else if (_sym == TK_CONTINUE)
    {
        getTok();
        jumpstmt = new ContinueStmtASTNode();
        check(TK_SEMICOLON);
    }
    else if (_sym == TK_BREAK)
    {
        getTok();
        jumpstmt = new BreakStmtASTNode();
        check(TK_SEMICOLON);
    }
    else if (_sym == TK_RETURN)
    {
        getTok();
        if (_sym != TK_SEMICOLON)
            expr = Expression();
        check(TK_SEMICOLON);
        jumpstmt = new ReturnStmtASTNode(_ast->integerTypeASTNode, expr);
        jumpstmt->setLineNum(_tok->line);
    }

    return jumpstmt;
}

ASTNode *CParser::ExpressionStatement()
{
    ASTNode *expr = NULL_AST_NODE;

    if (_sym != TK_SEMICOLON)
        expr = Expression();
    check(TK_SEMICOLON);

    return expr;
}

//
// Translation unit
//

ASTNode *CParser::TranslationUnit()
{
    ASTNode *extdecl = NULL_AST_NODE;
    ASTNode *tunit = NULL_AST_NODE;

    _stb.openScope();

    for (;;)
    {
        if (isStorageClassSpecifier(_sym) || isTypeQualifier(_sym) ||
            isTypeSpecifier(_sym, 1) ||
            _sym == TK___ASM || _sym == TK___ATTRIBUTE__ ||
            _sym == TK_INLINE || _sym == TK__NORETURN)
        {
            // _inTypedef is set in ExternalDeclaration/DeclartionSpecifiers/
            // StorageClassSpecifier
            extdecl = ExternalDeclaration();
            if (tunit == NULL_AST_NODE)
                tunit = new SequenceASTNode(extdecl);
            else
                static_cast<SequenceASTNode *>(tunit)->add(extdecl);

            //_inTypedef = false;
        }
        else if (_sym == TK_EOF)
        {
            break;
        }
        else
        {
            char msg[1024];
            sprintf(msg, "expected external declaration, found %d", _sym);
            parsingError(msg);
        }
    }

    static_cast<SequenceASTNode *>(tunit)->setScope(_stb.getTopScope());
    _stb.closeScope();

    return tunit;
}

ASTNode *CParser::ExternalDeclaration()
{
    // _inTypedef is set in DeclartionSpecifiers/StorageClassSpecifier
    ASTNode *declSpec = DeclarationSpecifiers();

    if (getLATok(2) == TK_LPAR || getLATok(3) == TK_LPAR)
    {
        return FunctionDefinition(declSpec);
    }
    else
    {
        ASTNode *decl;

        if (_inTypedef)
        {
            _inTypedef = false;
            decl = new TypeDeclASTNode(Declarator(declSpec), declSpec);
            decl->declare(_ast);
            check(TK_SEMICOLON);
        }
        else
        {
            decl = Declaration(declSpec);
        }

        // Traverse declarations i.e. insert into symbol table.
        //_ast->declare(decl);

        return decl;
    }
}

ASTNode *CParser::FunctionDefinition(ASTNode *funcType)
{
    ASTNode *funcName = NULL_AST_NODE;
    ASTNode *funcPrms = NULL_AST_NODE;
    // bool isPtrType = false;

    if (_sym == TK_TIMES)
    {
        getTok();
        funcType = new PointerTypeASTNode(funcType);
    }

    // Function name
    ASTNode *dummy = NULL_AST_NODE;
    funcName = Declarator(dummy);

    check(TK_LPAR);

    FunctionDeclASTNode *funcDecl = new FunctionDeclASTNode(funcType,
                                                            funcName,
                                                            NULL_AST_NODE,
                                                            NULL_AST_NODE);

    // TODO: Move ParameterTypeList to DirectDeclarator
    if (_sym != TK_RPAR)
    {
        funcPrms = ParameterTypeList();
        funcDecl->setPrms(funcPrms);
    }
    check(TK_RPAR);

    while (_sym == TK___ASM || _sym == TK___ATTRIBUTE__)
        GccDeclaratorExtension();

    //_ast->declare(funcDecl);

    _stb.openScope();

    // Parameters are going inside of the function body.
    // if (funcPrms != NULL_AST_NODE)
    //    _ast->declare(funcPrms);

    if (_sym != TK_SEMICOLON)
        funcDecl->setBody(FunctionBody());
    else
        getTok();

    funcDecl->setScope(_stb.getTopScope());
    _stb.closeScope();
    _ast->setCurrentFuncDecl(nullptr);

    return funcDecl;
}

} // namespace cparser
