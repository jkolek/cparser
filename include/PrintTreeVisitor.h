// Print tree visitor - header file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#ifndef PRINT_TREE_VISITOR
#define PRINT_TREE_VISITOR

#include "ASTNode.h"
#include "TreeVisitor.h"

namespace cparser
{

class PrintTreeVisitor : public TreeVisitor
{
    int _level; // Indentation level

public:
    PrintTreeVisitor() { _level = 0; }

    PrintTreeVisitor(int L) : _level(L) {}

    void visit(IdentASTNode *n);
    void visit(IntegerConstASTNode *n);
    void visit(StringConstASTNode *n);
    void visit(CharConstASTNode *n);
    void visit(SizeOfExprASTNode *n);
    void visit(TypeDeclASTNode *n);
    void visit(FunctionDeclASTNode *n);
    void visit(VarDeclASTNode *n);
    void visit(ParmDeclASTNode *n);
    void visit(FieldDeclASTNode *n);
    void visit(AsmStmtASTNode *n);
    void visit(BreakStmtASTNode *n);
    void visit(CaseLabelASTNode *n);
    void visit(CompoundStmtASTNode *n);
    void visit(ContinueStmtASTNode *n);
    void visit(DoStmtASTNode *n);
    void visit(ForStmtASTNode *n);
    void visit(GotoStmtASTNode *n);
    void visit(IfStmtASTNode *n);
    void visit(LabelStmtASTNode *n);
    void visit(ReturnStmtASTNode *n);
    void visit(SwitchStmtASTNode *n);
    void visit(WhileStmtASTNode *n);
    void visit(CastExprASTNode *n);
    void visit(BitNotExprASTNode *n);
    void visit(LogNotExprASTNode *n);
    void visit(PredecrementExprASTNode *n);
    void visit(PreincrementExprASTNode *n);
    void visit(PostdecrementExprASTNode *n);
    void visit(PostincrementExprASTNode *n);
    void visit(AddrExprASTNode *n);
    void visit(IndirectRefASTNode *n);
    void visit(NopExprASTNode *n);
    void visit(LShiftExprASTNode *n);
    void visit(RShiftExprASTNode *n);
    void visit(BitIorExprASTNode *n);
    void visit(BitXorExprASTNode *n);
    void visit(BitAndExprASTNode *n);
    void visit(LogAndExprASTNode *n);
    void visit(LogOrExprASTNode *n);
    void visit(PlusExprASTNode *n);
    void visit(MinusExprASTNode *n);
    void visit(MultExprASTNode *n);
    void visit(TruncDivExprASTNode *n);
    void visit(TruncModExprASTNode *n);
    void visit(ArrayRefASTNode *n);
    void visit(StructRefASTNode *n);
    void visit(LtExprASTNode *n);
    void visit(LeExprASTNode *n);
    void visit(GtExprASTNode *n);
    void visit(GeExprASTNode *n);
    void visit(EqExprASTNode *n);
    void visit(NeExprASTNode *n);
    void visit(AssignExprASTNode *n);
    void visit(CondExprASTNode *n);
    void visit(CallExprASTNode *n);
    void visit(VoidTypeASTNode *n);
    void visit(IntegralTypeASTNode *n);
    void visit(RealTypeASTNode *n);
    void visit(EnumeralTypeASTNode *n);
    void visit(PointerTypeASTNode *n);
    void visit(FunctionTypeASTNode *n);
    void visit(ArrayTypeASTNode *n);
    void visit(StructTypeASTNode *n);
    void visit(UnionTypeASTNode *n);
    void visit(NullASTNode *n);
    void visit(SequenceASTNode *n);
};

class CountCallExprVisitor : public TreeVisitor
{
    int callExprCount; // Number of call expressions

public:
    CountCallExprVisitor() { callExprCount = 0; }

    int getCallExprCount() { return callExprCount; }

    void visit(CallExprASTNode *n);
};

} // namespace cparser

#endif
