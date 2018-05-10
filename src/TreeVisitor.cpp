// Tree visitor - implementation file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include "../include/TreeVisitor.h"
#include "../include/ASTNode.h"

namespace cparser
{

void TreeVisitor::visit(IdentASTNode *n) {}
void TreeVisitor::visit(IntegerConstASTNode *n) {}
void TreeVisitor::visit(RealConstASTNode *n) {}
void TreeVisitor::visit(StringConstASTNode *n) {}
void TreeVisitor::visit(CharConstASTNode *n) {}

void TreeVisitor::visit(SizeOfExprASTNode *n) { n->getExpr()->accept(this); }
void TreeVisitor::visit(AlignOfExprASTNode *n) { n->getExpr()->accept(this); }

void TreeVisitor::visit(TypeDeclASTNode *n)
{
    n->getName()->accept(this);
    n->getBody()->accept(this);
}

void TreeVisitor::visit(FunctionDeclASTNode *n)
{
    n->getName()->accept(this);
    n->getType()->accept(this);
    n->getPrms()->accept(this);
    n->getBody()->accept(this);
}

void TreeVisitor::visit(VarDeclASTNode *n)
{
    n->getName()->accept(this);
    n->getType()->accept(this);
    n->getInit()->accept(this);
}

void TreeVisitor::visit(ParmDeclASTNode *n)
{
    n->getName()->accept(this);
    n->getType()->accept(this);
}

void TreeVisitor::visit(FieldDeclASTNode *n)
{
    n->getName()->accept(this);
    n->getType()->accept(this);
}

void TreeVisitor::visit(AsmStmtASTNode *n) {}

void TreeVisitor::visit(BreakStmtASTNode *n) {}

void TreeVisitor::visit(CaseLabelASTNode *n)
{
    n->getExpr()->accept(this);
    n->getStmt()->accept(this);
}

void TreeVisitor::visit(CompoundStmtASTNode *n)
{
    n->getDecls()->accept(this);
    n->getStmts()->accept(this);
}

void TreeVisitor::visit(ContinueStmtASTNode *n) {}

void TreeVisitor::visit(DoStmtASTNode *n)
{
    n->getCondition()->accept(this);
    n->getBody()->accept(this);
}

void TreeVisitor::visit(ForStmtASTNode *n)
{
    n->getInit()->accept(this);
    n->getCondition()->accept(this);
    n->getBody()->accept(this);
    n->getStep()->accept(this);
}

void TreeVisitor::visit(GotoStmtASTNode *n) {}

void TreeVisitor::visit(IfStmtASTNode *n)
{
    n->getCondition()->accept(this);
    n->getThenClause()->accept(this);
    n->getElseClause()->accept(this);
}

void TreeVisitor::visit(LabelStmtASTNode *n)
{
    n->getLabel()->accept(this);
    n->getStmt()->accept(this);
}

void TreeVisitor::visit(ReturnStmtASTNode *n) { n->getExpr()->accept(this); }

void TreeVisitor::visit(SwitchStmtASTNode *n)
{
    n->getExpr()->accept(this);
    n->getStmt()->accept(this);
}

void TreeVisitor::visit(WhileStmtASTNode *n)
{
    n->getCondition()->accept(this);
    n->getBody()->accept(this);
}

void TreeVisitor::visit(CastExprASTNode *n) { n->getExpr()->accept(this); }

void TreeVisitor::visit(BitNotExprASTNode *n) { n->getExpr()->accept(this); }

void TreeVisitor::visit(LogNotExprASTNode *n) {}

void TreeVisitor::visit(PredecrementExprASTNode *n)
{
    n->getExpr()->accept(this);
}

void TreeVisitor::visit(PreincrementExprASTNode *n)
{
    n->getExpr()->accept(this);
}

void TreeVisitor::visit(PostdecrementExprASTNode *n)
{
    n->getExpr()->accept(this);
}

void TreeVisitor::visit(PostincrementExprASTNode *n)
{
    n->getExpr()->accept(this);
}

void TreeVisitor::visit(AddrExprASTNode *n) { n->getExpr()->accept(this); }

void TreeVisitor::visit(IndirectRefASTNode *n) {}

void TreeVisitor::visit(NopExprASTNode *n) {}

void TreeVisitor::visit(LShiftExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(RShiftExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(BitIorExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(BitXorExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(BitAndExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(LogAndExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(LogOrExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(PlusExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(MinusExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(MultExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(TruncDivExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(TruncModExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(ArrayRefASTNode *n) {}
void TreeVisitor::visit(StructRefASTNode *n) {}

void TreeVisitor::visit(LtExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(LeExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(GtExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(GeExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(EqExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(NeExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(AssignExprASTNode *n)
{
    n->getLhs()->accept(this);
    n->getRhs()->accept(this);
}

void TreeVisitor::visit(CondExprASTNode *n) {}

void TreeVisitor::visit(CallExprASTNode *n)
{
    n->getExpr()->accept(this);
    n->getArgs()->accept(this);
}

void TreeVisitor::visit(VoidTypeASTNode *n) {}
void TreeVisitor::visit(IntegralTypeASTNode *n) {}
void TreeVisitor::visit(RealTypeASTNode *n) {}

void TreeVisitor::visit(EnumeralTypeASTNode *n)
{
    n->getName()->accept(this);
    n->getBody()->accept(this);
}

void TreeVisitor::visit(PointerTypeASTNode *n)
{
    n->getBaseType()->accept(this);
}

void TreeVisitor::visit(FunctionTypeASTNode *n)
{
    n->getType()->accept(this);
    n->getPrms()->accept(this);
}

void TreeVisitor::visit(ArrayTypeASTNode *n) {}

void TreeVisitor::visit(StructTypeASTNode *n)
{
    n->getName()->accept(this);
    n->getBody()->accept(this);
}

void TreeVisitor::visit(UnionTypeASTNode *n) {}
void TreeVisitor::visit(NullASTNode *n) {}

void TreeVisitor::visit(SequenceASTNode *n)
{
    std::vector<ASTNode *> elements = n->getElements();
    for (unsigned i = 0; i < elements.size(); i++)
        elements[i]->accept(this);
}

} // namespace cparser
