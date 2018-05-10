// Print tree visitor - implementation file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include "../include/PrintTreeVisitor.h"
#include "../include/ASTNode.h"
#include "../include/TreeVisitor.h"

#include <iostream>

#define TAB_SIZE 4

namespace cparser
{

static void printTab(int n)
{
    int i, x;

    x = n * TAB_SIZE;
    for (i = 0; i < x; i++)
        std::cout << " ";
}

void PrintTreeVisitor::visit(IdentASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_IDENT_NODE" << std::endl;
    printTab(_level);
    std::cout << "Value: " << n->getValue() << std::endl;
    _level--;
}

void PrintTreeVisitor::visit(IntegerConstASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_INTEGER_CONST" << std::endl;
    printTab(_level);
    std::cout << "Value: " << n->getValue() << std::endl;
    _level--;
}

void PrintTreeVisitor::visit(StringConstASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_STRING_CONST" << std::endl;
    printTab(_level);
    std::cout << "Value: " << n->getValue() << std::endl;
    _level--;
}

void PrintTreeVisitor::visit(CharConstASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_CHAR_CONST" << std::endl;
    printTab(_level);
    if (n->getValue() == '\n')
        std::cout << "Value: newline character" << std::endl;
    else
        std::cout << "Value: " << (char) n->getValue() << std::endl;
    _level--;
}

void PrintTreeVisitor::visit(SizeOfExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_SIZEOF_EXPR" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Expression:" << std::endl;
        n->getExpr()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(TypeDeclASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_TYPE_DECL" << std::endl;
    if (n->getName() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Type name:" << std::endl;
        n->getName()->accept(this);
    }
    if (n->getBody() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Body:" << std::endl;
        n->getBody()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(FunctionDeclASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_FUNCTION_DECL" << std::endl;
    if (n->getName() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Function name:" << std::endl;
        n->getName()->accept(this);
    }
    if (n->getType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Type:" << std::endl;
        n->getType()->accept(this);
    }
    if (n->getPrms() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Parameters:" << std::endl;
        n->getPrms()->accept(this);
    }
    if (n->getBody() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Body:" << std::endl;
        n->getBody()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(VarDeclASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_VAR_DECL" << std::endl;
    if (n->getName() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Name:" << std::endl;
        n->getName()->accept(this);
    }
    if (n->getType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Type:" << std::endl;
        n->getType()->accept(this);
    }
    if (n->getInit() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Init:" << std::endl;
        n->getInit()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(ParmDeclASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_PARM_DECL" << std::endl;
    if (n->getName() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Name:" << std::endl;
        n->getName()->accept(this);
    }
    if (n->getType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Type:" << std::endl;
        n->getType()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(FieldDeclASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_FIELD_DECL" << std::endl;
    if (n->getName() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Name:" << std::endl;
        n->getName()->accept(this);
    }
    if (n->getType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Type:" << std::endl;
        n->getType()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(AsmStmtASTNode *n)
{
    printTab(_level + 1);
    std::cout << "NK_ASM_STMT" << std::endl;
    printTab(_level + 1);
    std::cout << "Data:" << std::endl;
    printTab(_level + 2);
    std::cout << n->getData() << std::endl;
}

void PrintTreeVisitor::visit(BreakStmtASTNode *n)
{
    printTab(_level + 1);
    std::cout << "NK_BREAK_STMT" << std::endl;
}

void PrintTreeVisitor::visit(CaseLabelASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "CASE_LABEL" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Expression:" << std::endl;
        n->getExpr()->accept(this);
    }
    if (n->getStmt() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Statement:" << std::endl;
        n->getStmt()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(CompoundStmtASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_COMPOUND_STMT" << std::endl;
    if (n->getDecls() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Declarations:" << std::endl;
        n->getDecls()->accept(this);
    }
    if (n->getStmts() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Statements:" << std::endl;
        n->getStmts()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(ContinueStmtASTNode *n)
{
    printTab(_level + 1);
    std::cout << "NK_CONTINUE_STMT" << std::endl;
}

void PrintTreeVisitor::visit(DoStmtASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_DO_STMT" << std::endl;
    if (n->getCondition() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "condition:" << std::endl;
        n->getCondition()->accept(this);
    }
    if (n->getBody() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "body:" << std::endl;
        n->getBody()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(ForStmtASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_FOR_STMT" << std::endl;
    if (n->getInit() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Init:" << std::endl;
        n->getInit()->accept(this);
    }
    if (n->getCondition() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Condition:" << std::endl;
        n->getCondition()->accept(this);
    }
    if (n->getStep() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Step:" << std::endl;
        n->getStep()->accept(this);
    }
    if (n->getBody() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Body:" << std::endl;
        n->getBody()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(GotoStmtASTNode *n)
{
    //
}

void PrintTreeVisitor::visit(IfStmtASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_IF_STMT" << std::endl;
    if (n->getCondition() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Condition:" << std::endl;
        n->getCondition()->accept(this);
    }
    if (n->getThenClause() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "If clause:" << std::endl;
        n->getThenClause()->accept(this);
    }
    if (n->getElseClause() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Else clause:" << std::endl;
        n->getElseClause()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(LabelStmtASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "LABEL_STMT" << std::endl;
    if (n->getLabel() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Label:" << std::endl;
        n->getLabel()->accept(this);
    }
    if (n->getStmt() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Statement:" << std::endl;
        n->getStmt()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(ReturnStmtASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "RETURN_STMT" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Expression:" << std::endl;
        n->getExpr()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(SwitchStmtASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "SWITCH_STMT" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Expression:" << std::endl;
        n->getExpr()->accept(this);
    }
    if (n->getStmt() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Statement:" << std::endl;
        n->getStmt()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(WhileStmtASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "WHILE_STMT" << std::endl;
    if (n->getCondition() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Condition:" << std::endl;
        n->getCondition()->accept(this);
    }
    if (n->getBody() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Body:" << std::endl;
        n->getBody()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(CastExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_CAST_EXPR" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Expression:" << std::endl;
        n->getExpr()->accept(this);
    }
    if (n->getType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Type:" << std::endl;
        n->getType()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(BitNotExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_BIT_NOT_EXPR" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Expression:" << std::endl;
        n->getExpr()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(LogNotExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_LOG_NOT_EXPR" << std::endl;
    printTab(_level);
    std::cout << "Expression:" << std::endl;
    n->getExpr()->accept(this);
    _level--;
}

void PrintTreeVisitor::visit(PredecrementExprASTNode *n)
{
    // TODO: Implement.
}

void PrintTreeVisitor::visit(PreincrementExprASTNode *n)
{
    // TODO: Implement.
}

void PrintTreeVisitor::visit(PostdecrementExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_POSTDECREMENT_EXPR" << std::endl;
    printTab(_level);
    std::cout << "Expression:" << std::endl;
    n->getExpr()->accept(this);
    _level--;
}

void PrintTreeVisitor::visit(PostincrementExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_POSTINCREMENT_EXPR" << std::endl;
    printTab(_level);
    std::cout << "Expression:" << std::endl;
    n->getExpr()->accept(this);
    _level--;
}

void PrintTreeVisitor::visit(AddrExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_ADDR_EXPR" << std::endl;
    printTab(_level);
    std::cout << "Expression:" << std::endl;
    n->getExpr()->accept(this);
    _level--;
}

void PrintTreeVisitor::visit(IndirectRefASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_INDIRECT_REF" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Expression:" << std::endl;
        n->getExpr()->accept(this);
    }
    if (n->getField() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Field:" << std::endl;
        n->getField()->accept(this);
    }
    if (n->getType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Type:" << std::endl;
        n->getType()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(NopExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_NOP_EXPR" << std::endl;
    _level--;
}

void PrintTreeVisitor::visit(LShiftExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_LSHIFT_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(RShiftExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_RSHIFT_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(BitIorExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_BIT_IOR_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(BitXorExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_BIT_XOR_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(BitAndExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_BIT_AND_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(LogAndExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_LOG_AND_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(LogOrExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_LOG_OR_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(PlusExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_PLUS_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(MinusExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_MINUS_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(MultExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_MULT_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(TruncDivExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_TRUNC_DIV_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(TruncModExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_TRUNC_MOD_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(ArrayRefASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "ARRAY_REF" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Expression:" << std::endl;
        n->getExpr()->accept(this);
    }
    if (n->getIndex() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Index:" << std::endl;
        n->getIndex()->accept(this);
    }
    if (n->getType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Element type:" << std::endl;
        n->getType()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(StructRefASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "STRUCT_REF" << std::endl;
    if (n->getName() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Name:" << std::endl;
        n->getName()->accept(this);
    }
    if (n->getMember() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Member:" << std::endl;
        n->getMember()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(LtExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_LT_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(LeExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_LE_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(GtExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_GT_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(GeExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_GE_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(EqExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_EQ_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(NeExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_NE_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(AssignExprASTNode *n)
{
    _level++;

    printTab(_level);
    std::cout << "NK_ASSIGN_EXPR" << std::endl;

    printTab(_level);
    std::cout << "Lhs:" << std::endl;
    n->getLhs()->accept(this);

    printTab(_level);
    std::cout << "Rhs:" << std::endl;
    n->getRhs()->accept(this);

    _level--;
}

void PrintTreeVisitor::visit(CondExprASTNode *n) { /* TODO: Implement. */}

void PrintTreeVisitor::visit(CallExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_CALL_EXPR" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Function name:" << std::endl;
        n->getExpr()->accept(this);
    }
    if (n->getArgs() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Actual parameters:" << std::endl;
        n->getArgs()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(VoidTypeASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_VOID_TYPE" << std::endl;
    _level--;
}

void PrintTreeVisitor::visit(IntegralTypeASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_INTEGRAL_TYPE" << std::endl;
    printTab(_level);
    std::cout << "Alignment: " << n->getAlignment() << std::endl;
    printTab(_level);
    if (n->getIsSigned())
        std::cout << "Signed: true" << std::endl;
    else
        std::cout << "Signed: false" << std::endl;
    _level--;
}

void PrintTreeVisitor::visit(RealTypeASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_REAL_TYPE" << std::endl;
    printTab(_level);
    std::cout << "Alignment: " << n->getAlignment() << std::endl;
    printTab(_level);
    if (n->getIsDouble())
        std::cout << "Double: true" << std::endl;
    else
        std::cout << "Double: false" << std::endl;
    _level--;
}

void PrintTreeVisitor::visit(EnumeralTypeASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_ENUMERAL_TYPE" << std::endl;
    if (n->getName() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Name:" << std::endl;
        n->getName()->accept(this);
    }
    if (n->getBody() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Body:" << std::endl;
        n->getBody()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(PointerTypeASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_POINTER_TYPE" << std::endl;
    if (n->getBaseType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Base type:" << std::endl;
        n->getBaseType()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(FunctionTypeASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_FUNCTION_TYPE" << std::endl;
    if (n->getType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Type:" << std::endl;
        n->getType()->accept(this);
    }
    if (n->getPrms() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Parameters:" << std::endl;
        n->getPrms()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(ArrayTypeASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_ARRAY_TYPE" << std::endl;
    if (n->getExpr() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Expression:" << std::endl;
        n->getExpr()->accept(this);
    }
    if (n->getElementType() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Element type:" << std::endl;
        n->getElementType()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(StructTypeASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_STRUCT_TYPE" << std::endl;
    if (n->getName() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Name:" << std::endl;
        n->getName()->accept(this);
    }
    if (n->getBody() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "Body:" << std::endl;
        n->getBody()->accept(this);
    }
    _level--;
}

void PrintTreeVisitor::visit(UnionTypeASTNode *n) { /* TODO: Implement. */}

void PrintTreeVisitor::visit(NullASTNode *n)
{
    printTab(_level + 1);
    std::cout << "NK_UNKNOWN" << std::endl;
}

void PrintTreeVisitor::visit(SequenceASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_LIST" << std::endl;
    std::vector<ASTNode *> &elements = n->getElements();

    for (unsigned i = 0; i < elements.size(); i++)
    {
        printTab(_level);
        std::cout << "Element "<< i << ":" << std::endl;
        if (elements[i] != nullptr && elements[i] != NULL_AST_NODE)
            elements[i]->accept(this);
    }
    _level--;
}

void CountCallExprVisitor::visit(CallExprASTNode *n) { callExprCount++; }

} // namespace cparser
