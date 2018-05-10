// ASTNode classes of abstract syntax tree.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#ifndef AST_NODE_H
#define AST_NODE_H

#include "../include/SymbolTable.h"

#include <vector>
#include <cstdio>
#include <cstring>
#include <string>

#define ASSIGN_AST_NODE_REF(x, y)                                              \
    {                                                                          \
        x = y;                                                                 \
        if (x != NULL_AST_NODE)                                                \
            x->incRefCount();                                                  \
    }

#define PUSH_BACK_AST_NODE_REF(v, x)                                           \
    {                                                                          \
        if (x != NULL_AST_NODE)                                                \
        {                                                                      \
            v.push_back(x);                                                    \
            x->incRefCount();                                                  \
        }                                                                      \
    }

#define DELETE_AST_NODE_REF(x)                                                 \
    if (x != NULL_AST_NODE)                                                    \
    {                                                                          \
        x->decRefCount();                                                      \
        if (x->getRefCount() == 0)                                             \
            delete x;                                                          \
    }

/* TODO: Define the rest of macros. */
#define AST_PLUS(t, l, r) new PlusExprASTNode(t, l, r)
#define AST_MINUS(t, l, r) new MinusExprASTNode(t, l, r)
#define AST_MULT(t, l, r) new MultExprASTNode(t, l, r)

namespace cparser
{

enum ASTNodeKind
{
    NK_UNKNOWN,
    NK_IDENT_NODE,
    NK_LIST,
    NK_SIZEOF_EXPR,
    NK_ALIGNOF_EXPR,

    // Declarations
    NK_FUNCTION_DECL,
    NK_TYPE_DECL,
    NK_VAR_DECL,
    NK_PARM_DECL,
    NK_FIELD_DECL,

    // Statements
    NK_ASM_STMT,
    NK_BREAK_STMT,
    NK_CASE_LABEL,
    NK_COMPOUND_STMT,
    NK_CONTINUE_STMT,
    NK_DECL_STMT,
    NK_DO_STMT,
    NK_EXPR_STMT,
    NK_FILE_STMT,
    NK_FOR_STMT,
    NK_GOTO_STMT,
    NK_IF_STMT,
    NK_LABEL_STMT,
    NK_RETURN_STMT,
    NK_SCOPE_STMT,
    NK_SWITCH_STMT,
    NK_WHILE_STMT,

    // Expressions
    NK_INTEGER_CONST,
    NK_REAL_CONST,
    NK_COMPLEX_CONST,
    NK_STRING_CONST,
    NK_CHAR_CONST,
    NK_CAST_EXPR,
    NK_NEGATE_EXPR,
    NK_BIT_NOT_EXPR,
    NK_LOG_NOT_EXPR,
    NK_PREDECREMENT_EXPR,
    NK_PREINCREMENT_EXPR,
    NK_POSTDECREMENT_EXPR,
    NK_POSTINCREMENT_EXPR,
    NK_ADDR_EXPR,
    NK_INDIRECT_REF,
    NK_FIX_TRUNC_EXPR,
    NK_FLOAT_EXPR,
    NK_COMPLEX_EXPR,
    NK_NON_LVALUE_EXPR,
    NK_NOP_EXPR,
    NK_LSHIFT_EXPR,
    NK_RSHIFT_EXPR,
    NK_BIT_IOR_EXPR,
    NK_BIT_XOR_EXPR,
    NK_BIT_AND_EXPR,
    NK_LOG_AND_EXPR,
    NK_LOG_OR_EXPR,
    NK_PLUS_EXPR,
    NK_MINUS_EXPR,
    NK_MULT_EXPR,
    NK_TRUNC_DIV_EXPR,
    NK_TRUNC_MOD_EXPR,
    NK_RDIV_EXPR,
    NK_ARRAY_REF,
    NK_STRUCT_REF,
    NK_LT_EXPR,
    NK_LE_EXPR,
    NK_GT_EXPR,
    NK_GE_EXPR,
    NK_EQ_EXPR,
    NK_NE_EXPR,
    NK_ASSIGN_EXPR,
    NK_INIT_EXPR,
    NK_COMPOUND_EXPR,
    NK_COND_EXPR,
    NK_CALL_EXPR,

    // Input/output
    NK_GETC_EXPR,
    NK_PUTC_EXPR,
    NK_GETI_EXPR,
    NK_PUTI_EXPR,

    // Types
    NK_VOID_TYPE,
    NK_INTEGRAL_TYPE,
    NK_REAL_TYPE,
    NK_COMPLEX_TYPE,
    NK_ENUMERAL_TYPE,
    NK_BOOLEAN_TYPE,
    NK_POINTER_TYPE,
    NK_REFERENCE_TYPE,
    NK_FUNCTION_TYPE,
    NK_ARRAY_TYPE,
    NK_STRUCT_TYPE,
    NK_UNKNOWN_TYPE,
    NK_UNION_TYPE
};

class AbstractSyntaxTree;
class TreeVisitor;

class ASTNode
{
protected:
    ASTNodeKind kind;
    int lineNum;
    unsigned flags; // Additional flags
    unsigned refCount;

public:
    ASTNodeKind getKind() { return kind; }

    ASTNode()
    {
        flags = 0;
        refCount = 0;
        lineNum = 0;
        kind = NK_UNKNOWN;
    }

    ASTNode(ASTNodeKind Kind)
    {
        flags = 0;
        kind = Kind;
        refCount = 0;
        lineNum = 0;
    }

    // Virtual destructor
    virtual ~ASTNode() {}

    int getLineNum() { return lineNum; }
    void setLineNum(int LineNum) { lineNum = LineNum; }
    unsigned getFlags() { return flags; }
    void setFlags(unsigned Flags) { flags = Flags; }

    // Virtual functions
    virtual void declare(AbstractSyntaxTree *ast) {}
    virtual STType *checkType(AbstractSyntaxTree *ast) { return nullptr; }
    virtual void accept(TreeVisitor *v) {}

    bool isType()
    {
        return (kind == NK_VOID_TYPE || kind == NK_INTEGRAL_TYPE ||
                kind == NK_REAL_TYPE || kind == NK_COMPLEX_TYPE ||
                kind == NK_ENUMERAL_TYPE || kind == NK_BOOLEAN_TYPE ||
                kind == NK_POINTER_TYPE || kind == NK_REFERENCE_TYPE ||
                kind == NK_FUNCTION_TYPE || kind == NK_ARRAY_TYPE ||
                kind == NK_STRUCT_TYPE || kind == NK_UNKNOWN_TYPE ||
                kind == NK_UNION_TYPE);
    }

    void incRefCount() { refCount++; }
    void decRefCount() { refCount--; }
    unsigned getRefCount() { return refCount; }
};

// Singleton class
class NullASTNode : public ASTNode
{
    static NullASTNode *s_instance;

public:
    NullASTNode() { kind = NK_UNKNOWN; }

    ~NullASTNode() {}

    void accept(TreeVisitor *v);

    static NullASTNode *getInstance()
    {
        if (!s_instance)
            s_instance = new NullASTNode();
        return s_instance;
    }
};

#define NULL_AST_NODE NullASTNode::getInstance()

class IdentASTNode : public ASTNode
{
    std::string _value;

public:
    IdentASTNode(const std::string &VALUE)
    {
        _value = VALUE;
        kind = NK_IDENT_NODE;
    }

    IdentASTNode(const std::string &VALUE, int LineNum)
    {
        _value = VALUE;
        lineNum = LineNum;
        kind = NK_IDENT_NODE;
    }

    const char *getValue() { return _value.c_str(); }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class IntegerConstASTNode : public ASTNode
{
    int _value;

public:
    IntegerConstASTNode(int VALUE)
    {
        _value = VALUE;
        kind = NK_INTEGER_CONST;
    }

    int getValue() { return _value; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class RealConstASTNode : public ASTNode
{
    int _value;

public:
    RealConstASTNode(int VALUE)
    {
        _value = VALUE;
        kind = NK_REAL_CONST;
    }

    float getValue() { return _value; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class StringConstASTNode : public ASTNode
{
    std::string _value;
public:
    StringConstASTNode(const std::string &val)
    {
        _value = val;
        kind = NK_STRING_CONST;
    }

    const char *getValue() { return _value.c_str(); }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class CharConstASTNode : public ASTNode
{
    int _value;

public:
    CharConstASTNode(int VALUE)
    {
        _value = VALUE;
        kind = NK_CHAR_CONST;
    }

    int getValue() { return _value; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class SizeOfExprASTNode : public ASTNode
{
    ASTNode *_expr;

public:
    SizeOfExprASTNode(ASTNode *Expr)
    {
        kind = NK_SIZEOF_EXPR;
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~SizeOfExprASTNode()
    {
        DELETE_AST_NODE_REF(_expr);
    }

    ASTNode *getExpr() { return _expr; }


    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class AlignOfExprASTNode : public ASTNode
{
    ASTNode *_expr;

public:
    AlignOfExprASTNode(ASTNode *Expr)
    {
        kind = NK_SIZEOF_EXPR;
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~AlignOfExprASTNode()
    {
        DELETE_AST_NODE_REF(_expr);
    }

    ASTNode *getExpr() { return _expr; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class TypeDeclASTNode : public ASTNode
{
    ASTNode *_name;
    ASTNode *_body;

public:
    TypeDeclASTNode(ASTNode *Name, ASTNode *Body)
    {
        kind = NK_TYPE_DECL;
        ASSIGN_AST_NODE_REF(_name, Name);
        ASSIGN_AST_NODE_REF(_body, Body);
    }

    ~TypeDeclASTNode()
    {
        DELETE_AST_NODE_REF(_name);
        DELETE_AST_NODE_REF(_body);
    }

    void setName(ASTNode *Name) { ASSIGN_AST_NODE_REF(_name, Name); }
    void setBody(ASTNode *Body) { ASSIGN_AST_NODE_REF(_body, Body); }

    ASTNode *getName() { return _name; }
    ASTNode *getBody() { return _body; }

    void declare(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class FunctionDeclASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_name;
    ASTNode *_prms;
    ASTNode *_body;
    STScope *_scope;

public:
    FunctionDeclASTNode(ASTNode *tp, ASTNode *n, ASTNode *p, ASTNode *b)
    {
        kind = NK_FUNCTION_DECL;
        ASSIGN_AST_NODE_REF(_type, tp);
        ASSIGN_AST_NODE_REF(_name, n);
        ASSIGN_AST_NODE_REF(_prms, p);
        ASSIGN_AST_NODE_REF(_body, b);
        _scope = nullptr;
    }

    ~FunctionDeclASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_name);
        DELETE_AST_NODE_REF(_prms);
        DELETE_AST_NODE_REF(_body);
    }

    ASTNode *getType() { return _type; }
    ASTNode *getName() { return _name; }
    ASTNode *getPrms() { return _prms; }
    ASTNode *getBody() { return _body; }

    void setType(ASTNode *Type) { ASSIGN_AST_NODE_REF(_type, Type); }
    void setName(ASTNode *Name) { ASSIGN_AST_NODE_REF(_name, Name); }
    void setPrms(ASTNode *Prms) { ASSIGN_AST_NODE_REF(_prms, Prms); }
    void setBody(ASTNode *Body) { ASSIGN_AST_NODE_REF(_body, Body); }

    void setScope(STScope *s) { _scope = s; }
    STScope *getScope() { return _scope; }
    bool isForwardDeclaration() { return _body == NULL_AST_NODE; }

    void accept(TreeVisitor *v);
};

class VarDeclASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_name;
    ASTNode *_init;

public:
    VarDeclASTNode(ASTNode *Type, ASTNode *Name)
    {
        kind = NK_VAR_DECL;
        _init = NULL_AST_NODE;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_name, Name);
    }

    VarDeclASTNode(ASTNode *Type, ASTNode *Name, ASTNode *Init)
    {
        kind = NK_VAR_DECL;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_name, Name);
        ASSIGN_AST_NODE_REF(_init, Init);
    }

    ~VarDeclASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_name);
        DELETE_AST_NODE_REF(_init);
    }

    void setType(ASTNode *Type) { ASSIGN_AST_NODE_REF(_type, Type); }
    void setName(ASTNode *Name) { ASSIGN_AST_NODE_REF(_name, Name); }
    void setInit(ASTNode *Init) { ASSIGN_AST_NODE_REF(_init, Init); }

    ASTNode *getName() { return _name; }
    ASTNode *getType() { return _type; }
    ASTNode *getInit() { return _init; }

    void accept(TreeVisitor *v);
};

class ParmDeclASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_name;

public:
    ParmDeclASTNode(ASTNode *Type, ASTNode *Name)
    {
        kind = NK_PARM_DECL;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_name, Name);
    }

    ~ParmDeclASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_name);
    }

    ASTNode *getName() { return _name; }
    ASTNode *getType() { return _type; }

    //void declare(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class FieldDeclASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_name;

public:
    FieldDeclASTNode(ASTNode *Type, ASTNode *Name)
    {
        kind = NK_FIELD_DECL;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_name, Name);
    }

    ~FieldDeclASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_name);
    }

    void setType(ASTNode *Type) { ASSIGN_AST_NODE_REF(_type, Type); }
    void setName(ASTNode *Name) { ASSIGN_AST_NODE_REF(_name, Name); }

    ASTNode *getName() { return _name; }
    ASTNode *getType() { return _type; }

    //void declare(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class AsmStmtASTNode : public ASTNode
{
    std::string _data;

public:
    AsmStmtASTNode(const std::string &data)
    {
        _data = data;
    }

    ~AsmStmtASTNode()
    {
        //
    }

    const char *getData() { return _data.c_str(); }

    void accept(TreeVisitor *v);
};

class BreakStmtASTNode : public ASTNode
{
public:
    BreakStmtASTNode() { kind = NK_BREAK_STMT; }

    ~BreakStmtASTNode() {}


    void accept(TreeVisitor *v);
};

class CaseLabelASTNode : public ASTNode
{
    ASTNode *_expr;
    ASTNode *_stmt;

public:
    CaseLabelASTNode(ASTNode *Expr, ASTNode *Stmt)
    {
        kind = NK_CASE_LABEL;
        ASSIGN_AST_NODE_REF(_expr, Expr);
        ASSIGN_AST_NODE_REF(_stmt, Stmt);
    }

    ~CaseLabelASTNode()
    {
        DELETE_AST_NODE_REF(_expr);
        DELETE_AST_NODE_REF(_stmt);
    }

    ASTNode *getExpr() { return _expr; }
    ASTNode *getStmt() { return _stmt; }

    void setExpr(ASTNode *e)
    {
        DELETE_AST_NODE_REF(_expr);
        ASSIGN_AST_NODE_REF(_expr, e);
    }

    void setStmt(ASTNode *s)
    {
        DELETE_AST_NODE_REF(_stmt);
        ASSIGN_AST_NODE_REF(_stmt, s);
    }


    void accept(TreeVisitor *v);
};

class CompoundStmtASTNode : public ASTNode
{
    ASTNode *_decls;
    ASTNode *_stmts;
    STScope *_scope;

public:
    CompoundStmtASTNode(ASTNode *Decls, ASTNode *Stmts)
    {
        kind = NK_COMPOUND_STMT;
        ASSIGN_AST_NODE_REF(_decls, Decls);
        ASSIGN_AST_NODE_REF(_stmts, Stmts);
        _scope = nullptr;
    }

    ~CompoundStmtASTNode()
    {
        DELETE_AST_NODE_REF(_decls);
        DELETE_AST_NODE_REF(_stmts);
    }

    ASTNode *getDecls() { return _decls; }
    ASTNode *getStmts() { return _stmts; }
    void setDecls(ASTNode *Decls) { ASSIGN_AST_NODE_REF(_decls, Decls); }
    void setStmts(ASTNode *Stmts) { ASSIGN_AST_NODE_REF(_stmts, Stmts); }
    void setScope(STScope *s) { _scope = s; }
    STScope *getScope() { return _scope; }

    void declare(AbstractSyntaxTree *ast);

    void accept(TreeVisitor *v);
};

class ContinueStmtASTNode : public ASTNode
{
public:
    ContinueStmtASTNode() { kind = NK_CONTINUE_STMT; }

    ~ContinueStmtASTNode() {}


    void accept(TreeVisitor *v);
};

class DoStmtASTNode : public ASTNode
{
    ASTNode *_condition;
    ASTNode *_body;

public:
    DoStmtASTNode(ASTNode *Condition, ASTNode *Body)
    {
        kind = NK_DO_STMT;
        ASSIGN_AST_NODE_REF(_condition, Condition);
        ASSIGN_AST_NODE_REF(_body, Body);
    }

    ~DoStmtASTNode()
    {
        DELETE_AST_NODE_REF(_condition);
        DELETE_AST_NODE_REF(_body);
    }

    ASTNode *getCondition() { return _condition; }
    ASTNode *getBody() { return _body; }


    void accept(TreeVisitor *v);
};

class ForStmtASTNode : public ASTNode
{
    ASTNode *_init;
    ASTNode *_condition;
    ASTNode *_step;
    ASTNode *_body;

public:
    ForStmtASTNode(ASTNode *Init,
                   ASTNode *Condition,
                   ASTNode *Step,
                   ASTNode *Body)
    {
        kind = NK_FOR_STMT;
        ASSIGN_AST_NODE_REF(_init, Init);
        ASSIGN_AST_NODE_REF(_condition, Condition);
        ASSIGN_AST_NODE_REF(_step, Step);
        ASSIGN_AST_NODE_REF(_body, Body);
    }

    ~ForStmtASTNode()
    {
        DELETE_AST_NODE_REF(_init);
        DELETE_AST_NODE_REF(_condition);
        DELETE_AST_NODE_REF(_step);
        DELETE_AST_NODE_REF(_body);
    }

    ASTNode *getInit() { return _init; }
    ASTNode *getCondition() { return _condition; }
    ASTNode *getBody() { return _body; }
    ASTNode *getStep() { return _step; }


    void accept(TreeVisitor *v);
};

class GotoStmtASTNode : public ASTNode
{
    ASTNode *_label;

public:
    GotoStmtASTNode(ASTNode *Label)
    {
        kind = NK_GOTO_STMT;
        ASSIGN_AST_NODE_REF(_label, Label);
    }

    ~GotoStmtASTNode()
    {
        DELETE_AST_NODE_REF(_label);
    }

    ASTNode *getLabel()
    {
        return _label;
    }

    void accept(TreeVisitor *v);
};

class IfStmtASTNode : public ASTNode
{
    ASTNode *_condition;
    ASTNode *_thenClause;
    ASTNode *_elseClause;

public:
    IfStmtASTNode(ASTNode *Condition, ASTNode *ThenClause, ASTNode *ElseClause)
    {
        kind = NK_IF_STMT;
        ASSIGN_AST_NODE_REF(_condition, Condition);
        ASSIGN_AST_NODE_REF(_thenClause, ThenClause);
        ASSIGN_AST_NODE_REF(_elseClause, ElseClause);
    }

    ~IfStmtASTNode()
    {
        DELETE_AST_NODE_REF(_condition);
        DELETE_AST_NODE_REF(_thenClause);
        DELETE_AST_NODE_REF(_elseClause);
    }

    ASTNode *getCondition() { return _condition; }
    ASTNode *getThenClause() { return _thenClause; }
    ASTNode *getElseClause() { return _elseClause; }


    void accept(TreeVisitor *v);
};

class LabelStmtASTNode : public ASTNode
{
    ASTNode *_label;
    ASTNode *_stmt;

public:
    LabelStmtASTNode(ASTNode *Label, ASTNode *Stmt)
    {
        kind = NK_LABEL_STMT;
        ASSIGN_AST_NODE_REF(_label, Label);
        ASSIGN_AST_NODE_REF(_stmt, Stmt);
    }

    ~LabelStmtASTNode()
    {
        DELETE_AST_NODE_REF(_label);
        DELETE_AST_NODE_REF(_stmt);
    }

    ASTNode *getLabel() { return _label; }
    ASTNode *getStmt() { return _stmt; }


    void accept(TreeVisitor *v);
};

class ReturnStmtASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_expr;

public:
    ReturnStmtASTNode(ASTNode *Type, ASTNode *Expr)
    {
        kind = NK_RETURN_STMT;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~ReturnStmtASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_expr);
    }

    ASTNode *getExpr() { return _expr; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class SwitchStmtASTNode : public ASTNode
{
    ASTNode *_expr;
    ASTNode *_stmt;

public:
    SwitchStmtASTNode(ASTNode *Expr, ASTNode *Stmt)
    {
        kind = NK_SWITCH_STMT;
        ASSIGN_AST_NODE_REF(_expr, Expr);
        ASSIGN_AST_NODE_REF(_stmt, Stmt);
    }

    ~SwitchStmtASTNode()
    {
        DELETE_AST_NODE_REF(_expr);
        DELETE_AST_NODE_REF(_stmt);
    }

    ASTNode *getExpr() { return _expr; }
    ASTNode *getStmt() { return _stmt; }


    void accept(TreeVisitor *v);
};

class WhileStmtASTNode : public ASTNode
{
    ASTNode *_condition;
    ASTNode *_body;

public:
    WhileStmtASTNode(ASTNode *Condition, ASTNode *Body)
    {
        kind = NK_WHILE_STMT;
        ASSIGN_AST_NODE_REF(_condition, Condition);
        ASSIGN_AST_NODE_REF(_body, Body);
    }

    ~WhileStmtASTNode()
    {
        DELETE_AST_NODE_REF(_condition);
        DELETE_AST_NODE_REF(_body);
    }

    ASTNode *getCondition() { return _condition; }
    ASTNode *getBody() { return _body; }


    void accept(TreeVisitor *v);
};

class CastExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_expr;

public:
    CastExprASTNode(ASTNode *Type, ASTNode *Expr)
    {
        kind = NK_CAST_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~CastExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_expr);
    }

    ASTNode *getType() { return _type; }
    ASTNode *getExpr() { return _expr; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class BitNotExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_expr;

public:
    BitNotExprASTNode(ASTNode *Type, ASTNode *Expr)
    {
        kind = NK_BIT_NOT_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~BitNotExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_expr);
    }

    ASTNode *getExpr() { return _expr; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class LogNotExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_expr;

public:
    LogNotExprASTNode(ASTNode *Type, ASTNode *Expr)
    {
        kind = NK_LOG_NOT_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~LogNotExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_expr);
    }

    ASTNode *getExpr() { return _expr; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class PredecrementExprASTNode : public ASTNode
{
    ASTNode *_expr;

public:
    PredecrementExprASTNode(ASTNode *Expr)
    {
        kind = NK_PREDECREMENT_EXPR;
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~PredecrementExprASTNode() { DELETE_AST_NODE_REF(_expr); }

    ASTNode *getExpr() { return _expr; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class PreincrementExprASTNode : public ASTNode
{
    ASTNode *_expr;

public:
    PreincrementExprASTNode(ASTNode *Expr)
    {
        kind = NK_PREINCREMENT_EXPR;
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~PreincrementExprASTNode() { DELETE_AST_NODE_REF(_expr); }

    ASTNode *getExpr() { return _expr; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class PostdecrementExprASTNode : public ASTNode
{
    ASTNode *_expr;

public:
    PostdecrementExprASTNode(ASTNode *Expr)
    {
        kind = NK_POSTDECREMENT_EXPR;
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~PostdecrementExprASTNode() { DELETE_AST_NODE_REF(_expr); }

    ASTNode *getExpr() { return _expr; }


    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class PostincrementExprASTNode : public ASTNode
{
    ASTNode *_expr;

public:
    PostincrementExprASTNode(ASTNode *Expr)
    {
        kind = NK_POSTINCREMENT_EXPR;
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~PostincrementExprASTNode() { DELETE_AST_NODE_REF(_expr); }

    ASTNode *getExpr() { return _expr; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class AddrExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_expr;

public:
    AddrExprASTNode(ASTNode *Type, ASTNode *Expr)
    {
        kind = NK_ADDR_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~AddrExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_expr);
    }

    ASTNode *getExpr() { return _expr; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class IndirectRefASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_expr;
    ASTNode *_field;

public:
    IndirectRefASTNode(ASTNode *Type, ASTNode *Expr, ASTNode *Field)
    {
        kind = NK_INDIRECT_REF;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_expr, Expr);
        ASSIGN_AST_NODE_REF(_field, Field);
    }

    ~IndirectRefASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_expr);
        DELETE_AST_NODE_REF(_field);
    }

    ASTNode *getType() { return _type; }
    ASTNode *getExpr() { return _expr; }
    ASTNode *getField() { return _field; }

    void setExpr(ASTNode *e)
    {
        DELETE_AST_NODE_REF(_expr);
        ASSIGN_AST_NODE_REF(_expr, e);
    }

    void setField(ASTNode *f)
    {
        DELETE_AST_NODE_REF(_field);
        ASSIGN_AST_NODE_REF(_field, f);
    }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class NopExprASTNode : public ASTNode
{
public:
    NopExprASTNode() { kind = NK_NOP_EXPR; }

    ~NopExprASTNode() {}

    // //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class LShiftExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    LShiftExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_LSHIFT_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~LShiftExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class RShiftExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    RShiftExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_RSHIFT_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~RShiftExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class BitIorExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    BitIorExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_BIT_IOR_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~BitIorExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class BitXorExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    BitXorExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_BIT_XOR_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~BitXorExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class BitAndExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    BitAndExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_BIT_AND_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~BitAndExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class LogAndExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    LogAndExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_LOG_AND_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~LogAndExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class LogOrExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    LogOrExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_LOG_OR_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~LogOrExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class PlusExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    PlusExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_PLUS_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~PlusExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class MinusExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    MinusExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_MINUS_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~MinusExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class MultExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    MultExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_MULT_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~MultExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class TruncDivExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    TruncDivExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_TRUNC_DIV_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~TruncDivExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class TruncModExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    TruncModExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_TRUNC_MOD_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~TruncModExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class ArrayRefASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_expr;
    ASTNode *_index;

public:
    ArrayRefASTNode(ASTNode *Type, ASTNode *Expr, ASTNode *Index)
    {
        kind = NK_ARRAY_REF;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_expr, Expr);
        ASSIGN_AST_NODE_REF(_index, Index);
    }

    ~ArrayRefASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_expr);
        DELETE_AST_NODE_REF(_index);
    }

    ASTNode *getType() { return _type; }
    ASTNode *getExpr() { return _expr; }
    ASTNode *getIndex() { return _index; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class StructRefASTNode : public ASTNode
{
    ASTNode *_name;
    ASTNode *_member;

public:
    StructRefASTNode(ASTNode *Name, ASTNode *Member)
    {
        kind = NK_STRUCT_REF;
        ASSIGN_AST_NODE_REF(_name, Name);
        ASSIGN_AST_NODE_REF(_member, Member);
    }

    ~StructRefASTNode()
    {
        DELETE_AST_NODE_REF(_name);
        DELETE_AST_NODE_REF(_member);
    }

    void setName(ASTNode *Name) { _name = Name; }
    void setMember(ASTNode *Member) { _member = Member; }

    ASTNode *getName() { return _name; }
    ASTNode *getMember() { return _member; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class LtExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    LtExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_LT_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~LtExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class LeExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    LeExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_LE_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~LeExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class GtExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    GtExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_GT_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~GtExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class GeExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    GeExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_GE_EXPR;

        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~GeExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class EqExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    EqExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_EQ_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~EqExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class NeExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    NeExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_NE_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~NeExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class AssignExprASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_lhs;
    ASTNode *_rhs;

public:
    AssignExprASTNode(ASTNode *Type, ASTNode *Lhs, ASTNode *Rhs)
    {
        kind = NK_ASSIGN_EXPR;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_lhs, Lhs);
        ASSIGN_AST_NODE_REF(_rhs, Rhs);
    }

    ~AssignExprASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_lhs);
        DELETE_AST_NODE_REF(_rhs);
    }

    ASTNode *getLhs() { return _lhs; }
    ASTNode *getRhs() { return _rhs; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class CondExprASTNode : public ASTNode
{
    ASTNode *_condition;
    ASTNode *_thenClause;
    ASTNode *_elseClause;

public:
    CondExprASTNode(ASTNode *Condition, ASTNode *thenClause, ASTNode *elseClause)
    {
        kind = NK_COND_EXPR;
        ASSIGN_AST_NODE_REF(_condition, Condition);
        ASSIGN_AST_NODE_REF(_thenClause, thenClause);
        ASSIGN_AST_NODE_REF(_elseClause, elseClause);
    }

    ~CondExprASTNode()
    {
        DELETE_AST_NODE_REF(_condition);
        DELETE_AST_NODE_REF(_thenClause);
        DELETE_AST_NODE_REF(_elseClause);
    }

    ASTNode *getCondition() { return _condition; }
    ASTNode *getThenClause() { return _thenClause; }
    ASTNode *getElseClause() { return _elseClause; }

    void accept(TreeVisitor *v);
};

class CallExprASTNode : public ASTNode
{
    ASTNode *_expr;
    ASTNode *_args;

public:
    CallExprASTNode(ASTNode *Expr, ASTNode *Args)
    {
        kind = NK_CALL_EXPR;
        ASSIGN_AST_NODE_REF(_expr, Expr);
        ASSIGN_AST_NODE_REF(_args, Args);
    }

    ~CallExprASTNode()
    {
        DELETE_AST_NODE_REF(_expr);
        DELETE_AST_NODE_REF(_args);
    }

    ASTNode *getExpr() { return _expr; }
    ASTNode *getArgs() { return _args; }

    //STType *checkType(AbstractSyntaxTree *ast);
    void accept(TreeVisitor *v);
};

class VoidTypeASTNode : public ASTNode
{
public:
    VoidTypeASTNode() { kind = NK_VOID_TYPE; }

    ~VoidTypeASTNode() {}

    void accept(TreeVisitor *v);
};

class IntegralTypeASTNode : public ASTNode
{
    unsigned _alignment;
    bool _isSigned;

public:
    IntegralTypeASTNode(unsigned Alignment, bool IsSigned)
        : _alignment(Alignment), _isSigned(IsSigned)
    {
        kind = NK_INTEGRAL_TYPE;
    }

    ~IntegralTypeASTNode() {}

    unsigned getAlignment() { return _alignment; }
    bool getIsSigned() { return _isSigned; }

    void accept(TreeVisitor *v);
};

class RealTypeASTNode : public ASTNode
{
    unsigned _alignment;
    bool _isDouble;

public:
    RealTypeASTNode(unsigned Alignment, bool IsDouble)
        : _alignment(Alignment), _isDouble(IsDouble)
    {
        kind = NK_REAL_TYPE;
    }

    ~RealTypeASTNode() {}

    unsigned getAlignment() { return _alignment; }
    bool getIsDouble() { return _isDouble; }

    void accept(TreeVisitor *v);
};

class EnumeralTypeASTNode : public ASTNode
{
    ASTNode *_name;
    ASTNode *_body;

public:
    EnumeralTypeASTNode(ASTNode *Name, ASTNode *Body)
    {
        kind = NK_ENUMERAL_TYPE;
        ASSIGN_AST_NODE_REF(_name, Name);
        ASSIGN_AST_NODE_REF(_body, Body);
    }

    ~EnumeralTypeASTNode()
    {
        DELETE_AST_NODE_REF(_name);
        DELETE_AST_NODE_REF(_body);
    }

    ASTNode *getName() { return _name; }
    ASTNode *getBody() { return _body; }

    void accept(TreeVisitor *v);
};

class PointerTypeASTNode : public ASTNode
{
    ASTNode *_baseType;

public:
    PointerTypeASTNode(ASTNode *BaseType)
    {
        kind = NK_POINTER_TYPE;
        ASSIGN_AST_NODE_REF(_baseType, BaseType);
    }

    ~PointerTypeASTNode() { DELETE_AST_NODE_REF(_baseType); }

    ASTNode *getBaseType() { return _baseType; }
    void setBaseType(ASTNode *BaseType) { _baseType = BaseType; }

    void accept(TreeVisitor *v);
};

class FunctionTypeASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_prms;

public:
    FunctionTypeASTNode(ASTNode *tp, ASTNode *p)
    {
        kind = NK_FUNCTION_TYPE;
        ASSIGN_AST_NODE_REF(_type, tp);
        ASSIGN_AST_NODE_REF(_prms, p);
    }

    ~FunctionTypeASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_prms);
    }

    ASTNode *getType() { return _type; }
    ASTNode *getPrms() { return _prms; }

    void setType(ASTNode *Type) { ASSIGN_AST_NODE_REF(_type, Type); }
    void setPrms(ASTNode *Prms) { ASSIGN_AST_NODE_REF(_prms, Prms); }

    void accept(TreeVisitor *v);
};

class ArrayTypeASTNode : public ASTNode
{
    ASTNode *_type;
    ASTNode *_expr;

public:
    ArrayTypeASTNode(ASTNode *Type, ASTNode *Expr)
    {
        kind = NK_ARRAY_TYPE;
        ASSIGN_AST_NODE_REF(_type, Type);
        ASSIGN_AST_NODE_REF(_expr, Expr);
    }

    ~ArrayTypeASTNode()
    {
        DELETE_AST_NODE_REF(_type);
        DELETE_AST_NODE_REF(_expr);
    }

    void setType(ASTNode *Type) { _type = Type; }
    ASTNode *getExpr() { return _expr; }
    ASTNode *getElementType() { return _type; }

    void accept(TreeVisitor *v);
};

class StructTypeASTNode : public ASTNode
{
    ASTNode *_name;
    ASTNode *_body;

public:
    StructTypeASTNode(ASTNode *Name, ASTNode *Body)
    {
        kind = NK_STRUCT_TYPE;
        ASSIGN_AST_NODE_REF(_name, Name);
        ASSIGN_AST_NODE_REF(_body, Body);
    }

    ~StructTypeASTNode()
    {
        DELETE_AST_NODE_REF(_name);
        DELETE_AST_NODE_REF(_body);
    }

    ASTNode *getName() { return _name; }
    ASTNode *getBody() { return _body; }

    void accept(TreeVisitor *v);
};

class UnionTypeASTNode : public ASTNode
{
    ASTNode *_name;
    ASTNode *_body;

public:
    UnionTypeASTNode(ASTNode *Name, ASTNode *Body)
    {
        kind = NK_UNION_TYPE;
        ASSIGN_AST_NODE_REF(_name, Name);
        ASSIGN_AST_NODE_REF(_body, Body);
    }

    ~UnionTypeASTNode()
    {
        DELETE_AST_NODE_REF(_name);
        DELETE_AST_NODE_REF(_body);
    }

    ASTNode *getName() { return _name; }
    ASTNode *getBody() { return _body; }

    void accept(TreeVisitor *v);
};

// List of trees
class SequenceASTNode : public ASTNode
{
    STScope *_scope;
    std::vector<ASTNode *> _elements;

public:
    SequenceASTNode()
    {
        kind = NK_LIST;
        _scope = nullptr;
    }

    SequenceASTNode(ASTNode *n)
    {
        kind = NK_LIST;
        add(n);
    }

    ~SequenceASTNode()
    {
        for (unsigned i = 0; i < _elements.size(); i++)
            DELETE_AST_NODE_REF(_elements[i]);
    }

    // void setElements(ASTNode *Elements) { elements = Elements; }
    std::vector<ASTNode *> &getElements() { return _elements; }
    void setElements(std::vector<ASTNode *> e) { _elements = e; }

    void add(ASTNode *n)
    {
        // If n is a tree list, append its elements to these elements
        if (n->getKind() == NK_LIST)
        {
            std::vector<ASTNode *> &tmpVec =
                static_cast<SequenceASTNode *>(n)->getElements();

            for (unsigned i = 0; i < tmpVec.size(); i++)
                PUSH_BACK_AST_NODE_REF(_elements, tmpVec[i]);

            // We don't need n anymore.
            delete n;
        }
        else
        {
            PUSH_BACK_AST_NODE_REF(_elements, n);
        }
    }

    void addElement(ASTNode *e) { PUSH_BACK_AST_NODE_REF(_elements, e); }

    unsigned size() { return _elements.size(); }
    void setScope(STScope *s) { _scope = s; }
    STScope *getScope() { return _scope; }

    void declare(AbstractSyntaxTree *ast);

    void accept(TreeVisitor *v);
};

} // namespace cparser

#endif
