// Symbol table - header file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "common.h"
#include <map>
#include <vector>

namespace cparser
{

// Object kinds
enum STObjectKind
{
    STOK_CON,
    STOK_VAR,
    STOK_VAR_STATIC,
    STOK_TYPE,
    STOK_FUNC,
    STOK_PAR,
    STOK_LAB,
    STOK_PTR
};

// Type kinds
enum STTypeKind
{
    STTK_NONE,
    STTK_CHAR,
    STTK_SHORT,
    STTK_INT,
    STTK_UNSIGNED,
    STTK_LONG,
    STTK_FLOAT,
    STTK_DOUBLE,
    STTK_VOID,
    STTK_ARRAY,
    STTK_STRUCT,
    STTK_UNION,
    STTK_BOOL,
    STTK_REAL,
    STTK_POINTER,
    STTK_ENUM,
    STTK_FUNCTION
};

struct STType;
struct STObject;
struct STScope;

struct STType
{
    STTypeKind kind;
    struct STType *elemType;    // Array element type
    struct STType *baseType;    // Base type of pointer type
    struct STType *funcType;    // Function pointer type
    unsigned nFields;           // Number of struct fields
    unsigned length;            // Array length
    struct STObject *fields;    // Struct fields
    unsigned size;
    bool isSigned;

    STType(STTypeKind Kind)
        : kind(Kind), elemType(nullptr), baseType(nullptr),
          funcType(nullptr), nFields(0), length(0), fields(nullptr),
          size(0), isSigned(false)
    {
    }
};

struct STObject
{
    STObjectKind kind;
    char name[IDLEN];
    STType *type;

    int ival;  // Integer constant value.
    int level; // Scope level

    // Function specific attributes
    unsigned prmc; // Function parameter count
    struct STObject *locals; // Function parameters and local variables

    bool isConstant;

    // Next object in a list
    struct STObject *next;

    STObject(const char *Name, STObjectKind Kind, STType *Type)
        : ival(0), level(0), prmc(0), locals(nullptr),
          isConstant(false), next(nullptr)
    {
        strcpy(name, Name);
        kind = Kind;
        type = Type;
    }
};

struct STScope
{
    struct STScope *outer; // Pointer to the next outer scope
    STObject *locals;     // Pointer to the objects in this scope
    int nVars;           // Number of variables in this scope
    int nPars;           // Number of variables in this scope
    int size;            // Size of scope in bytes

    STScope() : outer(nullptr), locals(nullptr), nVars(0), nPars(0), size(0) {}
};

class SymbolTable
{
    STScope *topScope;    // Current scope
    STScope *globalScope; // Current scope
    int level;           // (0 = global, 1 >= local)
    std::vector<STScope *> scopePool;
    std::vector<STObject *> objectPool;
    std::vector<STType *> typePool;

public:
    // Predefined types
    STType *charType;
    STType *shortType;
    STType *intType;
    STType *unsignedType;
    STType *longType;
    STType *floatType;
    STType *doubleType;
    STType *voidType;
    STType *nullType;
    STType *noType;

    STObject *noObj;

    // Allocators
    STType *allocType(STTypeKind kind);
    STObject *allocObject(const char *, STObjectKind, STType *);
    STScope *allocScope();

    // Symbol table interface
    STObject *insert(const char *, STObjectKind, STType *);
    STObject *insertGlobalVariable(const char *, STType *, const char *);
    STObject *find(const char *);

    int getLevel() { return level; }

    void openScope(void);
    void closeScope(void);
    void setTopScope(STScope *s);
    STScope *getTopScope();

    bool isIntegralType(STType *);
    bool isRealType(STType *);
    bool isArithmeticType(STType *);
    bool isPointerType(STType *);
    bool isScalarType(STType *);
    bool isFunctionPointerType(STType *);
    bool equalFunctionPointerTypes(STType *, STType *);
    bool equalBasePointerTypes(STType *, STType *);

    bool compatible(STType *, STType *);
    bool assignable(STType *, STType *);
    bool convertible(STType *, STType *);

    SymbolTable();
    ~SymbolTable();
};

} // namespace cparser

#endif
