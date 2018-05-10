// Abstract syntax tree - implementation file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include "../include/AbstractSyntaxTree.h"
#include "../include/ASTNode.h"
#include "../include/PrintTreeVisitor.h"
#include "../include/TreeVisitor.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

namespace cparser
{

void AbstractSyntaxTree::error(int lineNum, const char *format, ...)
{
    char dest[4096];

    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);
    printf("error: %s; line %d\n", dest, lineNum);

    exit(1);
}

void AbstractSyntaxTree::warning(int lineNum, const char *format, ...)
{
    char dest[4096];

    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);
    printf("warning: %s; line %d\n", dest, lineNum);
    _warnings++;
}

void AbstractSyntaxTree::visit(TreeVisitor *visitor)
{
    if (_root->getKind() == NK_LIST)
        _stb->setTopScope(static_cast<SequenceASTNode *>(_root)->getScope());

    visitor->visit(static_cast<SequenceASTNode *>(_root));

    if (_root->getKind() == NK_LIST)
        _stb->setTopScope(nullptr);
}

STType *AbstractSyntaxTree::getStbType(ASTNode *typeASTNode)
{
    switch (typeASTNode->getKind())
    {
        case NK_VOID_TYPE:
            return _stb->voidType;
        case NK_INTEGRAL_TYPE:
        {
            IntegralTypeASTNode *intTypeASTNode =
                static_cast<IntegralTypeASTNode *>(typeASTNode);

            if (intTypeASTNode->getIsSigned())
            {
                switch (intTypeASTNode->getAlignment())
                {
                    case 1:
                        return _stb->charType;
                    case 2:
                        return _stb->shortType;
                    case 4:
                        return _stb->intType;
                    case 8:
                        return _stb->longType;
                    default:
                        return _stb->noType;
                }
            }
            else
            {
                switch (intTypeASTNode->getAlignment())
                {
                    // case 1:  return stb->charType;
                    // case 2:  return stb->shortType;
                    case 4:
                        return _stb->unsignedType;
                    default:
                        return _stb->noType;
                }
            }
        }
        break;
        case NK_REAL_TYPE:
        {
            RealTypeASTNode *realTypeASTNode =
                static_cast<RealTypeASTNode *>(typeASTNode);

            return realTypeASTNode->getIsDouble() ? _stb->doubleType
                                                  : _stb->floatType;
        }
        case NK_COMPLEX_TYPE:
            return _stb->noType;
        case NK_ENUMERAL_TYPE:
            return _stb->noType;
        case NK_BOOLEAN_TYPE:
            return _stb->noType;
        case NK_POINTER_TYPE:
        {
            STType *type = _stb->allocType(STTK_POINTER);
            PointerTypeASTNode *ptrTypeASTNode =
                static_cast<PointerTypeASTNode *>(typeASTNode);
            type->baseType = getStbType(ptrTypeASTNode->getBaseType());
            type->size = 4; // Pointer type size is 4 bytes.
            return type;
        }
        case NK_REFERENCE_TYPE:
            return _stb->noType;
        case NK_FUNCTION_TYPE:
        {
            STType *type = _stb->allocType(STTK_FUNCTION);
            FunctionDeclASTNode *funcTypeASTNode =
                static_cast<FunctionDeclASTNode *>(typeASTNode);

            type->funcType = getStbType(funcTypeASTNode->getType());
            type->size = 4;
            return type;
        }
        case NK_ARRAY_TYPE:
        {
            STType *type = _stb->allocType(STTK_ARRAY);
            ArrayTypeASTNode *arrTypeASTNode =
                static_cast<ArrayTypeASTNode *>(typeASTNode);
            type->elemType = getStbType(arrTypeASTNode->getElementType());

            if (AST_MATCH_INTEGER_CONST(arrTypeASTNode->getExpr()))
                type->size = type->elemType->size *
                             AST_INTEGER_CONST_VALUE(arrTypeASTNode->getExpr());

            return type;
        }
        case NK_STRUCT_TYPE:
        {
            STType *type = nullptr;
            // FIXME: At this point a record type should already be in the
            // symbol table.
            StructTypeASTNode *recTypeASTNode =
                static_cast<StructTypeASTNode *>(typeASTNode);

            if (AST_MATCH_IDENT(recTypeASTNode->getName()))
            {
                const char *recordName =
                    AST_IDENT_VALUE(recTypeASTNode->getName());
                unsigned recordLineNum =
                    AST_IDENT_LINE_NUM(recTypeASTNode->getName());
                STObject *obj = _stb->find(recordName);

                if (obj == _stb->noObj)
                    // Record type declaration is not found in symbol table.
                    error(recordLineNum,
                          "struct type '%s' not declared",
                          recordName);
                else
                    type = obj->type;
            }
            return type;
        }
        case NK_UNION_TYPE:
        {
            STType *type = nullptr;
            // FIXME: At this point a union type should already be in the
            // symbol table.
            UnionTypeASTNode *unionTypeASTNode =
                static_cast<UnionTypeASTNode *>(typeASTNode);

            if (AST_MATCH_IDENT(unionTypeASTNode->getName()))
            {
                const char *unionName =
                    AST_IDENT_VALUE(unionTypeASTNode->getName());
                unsigned unionLineNum =
                    AST_IDENT_LINE_NUM(unionTypeASTNode->getName());
                STObject *obj = _stb->find(unionName);

                if (obj == _stb->noObj)
                    // Union type declaration is not found in symbol table.
                    error(unionLineNum,
                          "union type '%s' not declared",
                          unionName);
                else
                    type = obj->type;
            }
            return type;
        }
        case NK_UNKNOWN_TYPE:
            return _stb->noType;
        default:
            return _stb->noType;
    }
}

void AbstractSyntaxTree::declare()
{
    if (_root->getKind() == NK_LIST)
        _stb->setTopScope(static_cast<SequenceASTNode *>(_root)->getScope());

    _root->declare(this);

    if (_root->getKind() == NK_LIST)
        _stb->setTopScope(nullptr);
}

void TypeDeclASTNode::declare(AbstractSyntaxTree *ast)
{
    assert(_name != NULL_AST_NODE && "invalid type declaration");

    SymbolTable *stb = ast->getSymbolTable();
    const char *typeName = AST_IDENT_VALUE(_name);
    unsigned typeNameLineNum = AST_IDENT_LINE_NUM(_name);

    // FIXME: Using noType is only temporary solution
    STObject *obj = stb->insert(typeName, STOK_TYPE, stb->noType);

    if (obj == stb->noObj)
        ast->error(typeNameLineNum,
                   "type name '%s' already exists",
                   typeName);
}

void CompoundStmtASTNode::declare(AbstractSyntaxTree *ast)
{
    _decls->declare(ast);
}

void SequenceASTNode::declare(AbstractSyntaxTree *ast)
{
    for (unsigned i = 0; i < _elements.size(); i++)
        _elements[i]->declare(ast);
}

} // namespace cparser
