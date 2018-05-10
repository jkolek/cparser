// Tree visitor - header file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#ifndef TREE_VISITOR_H
#define TREE_VISITOR_H

#include "ASTNode.h"

namespace cparser
{

class TreeVisitor
{
public:
    // Virtual destructor
    virtual ~TreeVisitor() {}

    virtual void visit(IdentASTNode *n);
    virtual void visit(IntegerConstASTNode *n);
    virtual void visit(RealConstASTNode *n);
    virtual void visit(StringConstASTNode *n);
    virtual void visit(CharConstASTNode *n);
    virtual void visit(SizeOfExprASTNode *n);
    virtual void visit(AlignOfExprASTNode *n);
    virtual void visit(TypeDeclASTNode *n);
    virtual void visit(FunctionDeclASTNode *n);
    virtual void visit(VarDeclASTNode *n);
    virtual void visit(ParmDeclASTNode *n);
    virtual void visit(FieldDeclASTNode *n);
    virtual void visit(AsmStmtASTNode *n);
    virtual void visit(BreakStmtASTNode *n);
    virtual void visit(CaseLabelASTNode *n);
    virtual void visit(CompoundStmtASTNode *n);
    virtual void visit(ContinueStmtASTNode *n);
    virtual void visit(DoStmtASTNode *n);
    virtual void visit(ForStmtASTNode *n);
    virtual void visit(GotoStmtASTNode *n);
    virtual void visit(IfStmtASTNode *n);
    virtual void visit(LabelStmtASTNode *n);
    virtual void visit(ReturnStmtASTNode *n);
    virtual void visit(SwitchStmtASTNode *n);
    virtual void visit(WhileStmtASTNode *n);
    virtual void visit(CastExprASTNode *n);
    virtual void visit(BitNotExprASTNode *n);
    virtual void visit(LogNotExprASTNode *n);
    virtual void visit(PredecrementExprASTNode *n);
    virtual void visit(PreincrementExprASTNode *n);
    virtual void visit(PostdecrementExprASTNode *n);
    virtual void visit(PostincrementExprASTNode *n);
    virtual void visit(AddrExprASTNode *n);
    virtual void visit(IndirectRefASTNode *n);
    virtual void visit(NopExprASTNode *n);
    virtual void visit(LShiftExprASTNode *n);
    virtual void visit(RShiftExprASTNode *n);
    virtual void visit(BitIorExprASTNode *n);
    virtual void visit(BitXorExprASTNode *n);
    virtual void visit(BitAndExprASTNode *n);
    virtual void visit(LogAndExprASTNode *n);
    virtual void visit(LogOrExprASTNode *n);
    virtual void visit(PlusExprASTNode *n);
    virtual void visit(MinusExprASTNode *n);
    virtual void visit(MultExprASTNode *n);
    virtual void visit(TruncDivExprASTNode *n);
    virtual void visit(TruncModExprASTNode *n);
    virtual void visit(ArrayRefASTNode *n);
    virtual void visit(StructRefASTNode *n);
    virtual void visit(LtExprASTNode *n);
    virtual void visit(LeExprASTNode *n);
    virtual void visit(GtExprASTNode *n);
    virtual void visit(GeExprASTNode *n);
    virtual void visit(EqExprASTNode *n);
    virtual void visit(NeExprASTNode *n);
    virtual void visit(AssignExprASTNode *n);
    virtual void visit(CondExprASTNode *n);
    virtual void visit(CallExprASTNode *n);
    virtual void visit(VoidTypeASTNode *n);
    virtual void visit(IntegralTypeASTNode *n);
    virtual void visit(RealTypeASTNode *n);
    virtual void visit(EnumeralTypeASTNode *n);
    virtual void visit(PointerTypeASTNode *n);
    virtual void visit(FunctionTypeASTNode *n);
    virtual void visit(ArrayTypeASTNode *n);
    virtual void visit(StructTypeASTNode *n);
    virtual void visit(UnionTypeASTNode *n);
    virtual void visit(NullASTNode *n);
    virtual void visit(SequenceASTNode *n);
};

} // namespace cparser

#endif
