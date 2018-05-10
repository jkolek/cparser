// Symbol table - implementation file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include "../include/SymbolTable.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace cparser
{

STType *SymbolTable::allocType(STTypeKind kind)
{
    STType *type = new STType(kind);
    typePool.push_back(type);
    return type;
}

STObject *
SymbolTable::allocObject(const char *name, STObjectKind kind, STType *type)
{
    STObject *obj = new STObject(name, kind, type);
    objectPool.push_back(obj);
    return obj;
}

STScope *SymbolTable::allocScope()
{
    STScope *scope = new STScope();
    scopePool.push_back(scope);
    return scope;
}

SymbolTable::SymbolTable()
{
    level = -1;
    topScope = allocScope();
    topScope->outer = nullptr;
    globalScope = topScope;

    // Built-in types

    // char: 1 byte
    charType = allocType(STTK_CHAR);
    charType->size = 1;
    charType->isSigned = true;

    // char: 2 bytes
    shortType = allocType(STTK_SHORT);
    shortType->size = 2;
    shortType->isSigned = true;

    // int: 4 bytes
    intType = allocType(STTK_INT);
    intType->size = 4;
    intType->isSigned = true;

    // int: 4 bytes
    unsignedType = allocType(STTK_UNSIGNED);
    unsignedType->size = 4;
    unsignedType->isSigned = false;

    // int: 4 bytes
    longType = allocType(STTK_LONG);
    longType->size = 4;
    longType->isSigned = true;

    // float: 4 bytes
    floatType = allocType(STTK_FLOAT);
    floatType->size = 4;

    // double: 8 bytes
    doubleType = allocType(STTK_DOUBLE);
    doubleType->size = 8;

    // int: 4 bytes
    voidType = allocType(STTK_VOID);
    voidType->size = 4;

    noType = allocType(STTK_NONE);
    insert("NOTYPE", STOK_TYPE, noType);

    nullType = allocType(STTK_POINTER);

    // Dummy object
    noObj = allocObject("noObj", STOK_VAR, noType);

    // Built in types
    insert("__builtin_va_list", STOK_TYPE, noType);
}

SymbolTable::~SymbolTable()
{
    for (unsigned i = 0; i < scopePool.size(); i++)
        delete scopePool[i];

    for (unsigned i = 0; i < objectPool.size(); i++)
        delete objectPool[i];

    for (unsigned i = 0; i < typePool.size(); i++)
        delete typePool[i];
}

STObject *SymbolTable::insert(const char *name, STObjectKind kind, STType *type)
{
    // Create object node
    STObject *obj = allocObject(name, kind, type);
    STObject *p = nullptr, *last = nullptr;

    if (kind == STOK_VAR)
        topScope->nVars++;
    else if (kind == STOK_PAR)
        topScope->nPars++;

    obj->level = level;

    // Append object node
    p = topScope->locals;
    last = nullptr;
    while (p != nullptr)
    {
        if (strcmp(p->name, name) == 0)
            return noObj;
        last = p;
        p = p->next;
    }

    if (last == nullptr)
        topScope->locals = obj;
    else
        last->next = obj;

    return obj;
}

STObject *SymbolTable::find(const char *name)
{
    for (STScope *s = topScope; s != nullptr; s = s->outer)
        for (STObject *p = s->locals; p != nullptr; p = p->next)
            if (strcmp(p->name, name) == 0)
                return p;

    return noObj;
}

void SymbolTable::openScope(void)
{
    STScope *s = allocScope();

    s->outer = topScope;
    topScope = s;
    topScope->size = 0;
    level++;
}

void SymbolTable::closeScope(void)
{
    topScope = topScope->outer;
    level--;
}

void SymbolTable::setTopScope(STScope *s) { topScope = s; }

STScope *SymbolTable::getTopScope() { return topScope; }

bool SymbolTable::isIntegralType(STType *t)
{
    assert(t != nullptr && "Unexpected null pointer!");

    return (t == charType || t == intType || t == unsignedType ||
            t == shortType || t == longType);
}

bool SymbolTable::isRealType(STType *t)
{
    assert(t != nullptr && "Unexpected null pointer!");

    return (t == floatType || t == doubleType);
}

bool SymbolTable::isArithmeticType(STType *t)
{
    assert(t != nullptr && "Unexpected null pointer!");

    return (isIntegralType(t) || isRealType(t));
}

bool SymbolTable::isPointerType(STType *t)
{
    assert(t != nullptr && "Unexpected null pointer!");

    return t->kind == STTK_POINTER;
}

bool SymbolTable::isScalarType(STType *t)
{
    return isPointerType(t) || isArithmeticType(t);
}

bool SymbolTable::isFunctionPointerType(STType *t)
{
    return t->kind == STTK_POINTER &&
           t->baseType->kind == STTK_FUNCTION;
}

bool SymbolTable::equalFunctionPointerTypes(STType *t1, STType *t2)
{
    // TODO: Check if parameters of the function types match.
    return (isFunctionPointerType(t1) && isFunctionPointerType(t2) &&
            (t1->baseType->funcType == t2->baseType->funcType));
}

bool SymbolTable::equalBasePointerTypes(STType *t1, STType *t2)
{
    return (t1->kind == STTK_POINTER && t2->kind == STTK_POINTER &&
            (t1->baseType == t2->baseType));
}

// Two pointer types with the same type qualifiers are compatible if they
// point to objects of compatible type. The composite type for two compatible
// pointer types is the similarly qualified pointer to the composite type.

bool SymbolTable::compatible(STType *t1, STType *t2)
{
    assert(t1 != nullptr && t2 == nullptr && "Unexpected null pointer!");

    return ((t1 == t2) || (isIntegralType(t1) && isIntegralType(t2)) ||
            (isIntegralType(t1) && t2->kind == STTK_POINTER) ||
            (isIntegralType(t2) && t1->kind == STTK_POINTER) ||
            (t1->kind == STTK_POINTER && t2->baseType == noType) ||
            (t2->kind == STTK_POINTER && t1->baseType == noType) ||
            ((t1->kind == STTK_POINTER && t2->kind == STTK_POINTER) &&
             compatible(t1->baseType, t2->baseType)));
}

bool SymbolTable::assignable(STType *t1, STType *t2)
{
    assert(t1 != nullptr && t2 == nullptr && "Unexpected null pointer!");

    return ((isArithmeticType(t1) && isArithmeticType(t2)) ||
            (t1->kind == STTK_POINTER && t2->baseType == noType) ||
            (t2->kind == STTK_POINTER && t1->baseType == noType) ||
            ((t1->kind == STTK_POINTER && t2->kind == STTK_POINTER) &&
             (t1->baseType == t2->baseType)));
}

bool SymbolTable::convertible(STType *t1, STType *t2) { return true; }

} // namespace cparser
