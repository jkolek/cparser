// ASTNode - implementation file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include "../include/ASTNode.h"
#include "../include/TreeVisitor.h"

namespace cparser
{

NullASTNode *NullASTNode::s_instance = 0;

void IdentASTNode::accept(TreeVisitor *v) { v->visit(this); }

void IntegerConstASTNode::accept(TreeVisitor *v) { v->visit(this); }

void RealConstASTNode::accept(TreeVisitor *v) { v->visit(this); }

void StringConstASTNode::accept(TreeVisitor *v) { v->visit(this); }

void CharConstASTNode::accept(TreeVisitor *v) { v->visit(this); }

void SizeOfExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void AlignOfExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void TypeDeclASTNode::accept(TreeVisitor *v) { v->visit(this); }

void FunctionDeclASTNode::accept(TreeVisitor *v) { v->visit(this); }

void VarDeclASTNode::accept(TreeVisitor *v) { v->visit(this); }

void ParmDeclASTNode::accept(TreeVisitor *v) { v->visit(this); }

void FieldDeclASTNode::accept(TreeVisitor *v) { v->visit(this); }

void AsmStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void BreakStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void CaseLabelASTNode::accept(TreeVisitor *v) { v->visit(this); }

void CompoundStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void ContinueStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void DoStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void ForStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void GotoStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void IfStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void LabelStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void ReturnStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void SwitchStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void WhileStmtASTNode::accept(TreeVisitor *v) { v->visit(this); }

void CastExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void BitNotExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void LogNotExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void PredecrementExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void PreincrementExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void PostdecrementExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void PostincrementExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void AddrExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void IndirectRefASTNode::accept(TreeVisitor *v) { v->visit(this); }

void NopExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void LShiftExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void RShiftExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void BitIorExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void BitXorExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void BitAndExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void LogAndExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void LogOrExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void PlusExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void MinusExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void MultExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void TruncDivExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void TruncModExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void ArrayRefASTNode::accept(TreeVisitor *v) { v->visit(this); }

void StructRefASTNode::accept(TreeVisitor *v) { v->visit(this); }

void LtExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void LeExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void GtExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void GeExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void EqExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void NeExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void AssignExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void CondExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void CallExprASTNode::accept(TreeVisitor *v) { v->visit(this); }

void VoidTypeASTNode::accept(TreeVisitor *v) { v->visit(this); }

void IntegralTypeASTNode::accept(TreeVisitor *v) { v->visit(this); }

void RealTypeASTNode::accept(TreeVisitor *v) { v->visit(this); }

void EnumeralTypeASTNode::accept(TreeVisitor *v) { v->visit(this); }

void PointerTypeASTNode::accept(TreeVisitor *v) { v->visit(this); }

void FunctionTypeASTNode::accept(TreeVisitor *v) { v->visit(this); }

void ArrayTypeASTNode::accept(TreeVisitor *v) { v->visit(this); }

void StructTypeASTNode::accept(TreeVisitor *v) { v->visit(this); }

void UnionTypeASTNode::accept(TreeVisitor *v) { v->visit(this); }

void NullASTNode::accept(TreeVisitor *v) { v->visit(this); }

void SequenceASTNode::accept(TreeVisitor *v) { v->visit(this); }

} // namespace cparser
