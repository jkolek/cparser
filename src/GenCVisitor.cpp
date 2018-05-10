// Generate C visitor - implementation file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include "../include/GenCVisitor.h"
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

void GenCVisitor::visit(IdentASTNode *n)
{
    std::cout << n->getValue();
}

void GenCVisitor::visit(IntegerConstASTNode *n)
{
    std::cout << n->getValue();
}

void GenCVisitor::visit(StringConstASTNode *n)
{
    std::cout << "\"" << n->getValue() << "\"";
}

void GenCVisitor::visit(CharConstASTNode *n)
{
    if (n->getValue() == '\n')
        std::cout << "'\\n'";
    else
        std::cout << "'" << (char) n->getValue() << "'";
}

void GenCVisitor::visit(SizeOfExprASTNode *n)
{
    std::cout << "sizeof(";
    if (n->getExpr() != NULL_AST_NODE)
        n->getExpr()->accept(this);

    std::cout << ")";
}

void GenCVisitor::visit(AlignOfExprASTNode *n)
{
    std::cout << "_Alignof(";
    if (n->getExpr() != NULL_AST_NODE)
        n->getExpr()->accept(this);

    std::cout << ")";
}

void GenCVisitor::visit(TypeDeclASTNode *n)
{
    std::cout << "typedef ";
    n->getBody()->accept(this);
    std::cout << " ";
    n->getName()->accept(this);
}

void GenCVisitor::visit(FunctionDeclASTNode *n)
{
    n->getType()->accept(this);

    std::cout << " ";

    n->getName()->accept(this);

    std::cout << "(";
    _inList++;
    n->getPrms()->accept(this);
    _inList--;
    std::cout << ")" << std::endl;

    n->getBody()->accept(this);
}

void GenCVisitor::visit(VarDeclASTNode *n)
{
    // if (n->getType()->getKind() == NK_FUNCTION_TYPE)
    // {
    //     static_cast<FunctionTypeASTNode *>(n->getType())->getType()->accept(this);
    //     std::cout << "(*";
    //     n->getName()->accept(this);
    //     std::cout << ")";
    //     std::cout << "(";
    //     _inList++;
    //     static_cast<FunctionTypeASTNode *>(n->getType())->getPrms()->accept(this);
    //     _inList--;
    //     std::cout << ")";
    // }
    // else
    // {

    n->getType()->accept(this);

    std::cout << " ";

//    if (n->getType()->getKind() == NK_POINTER_TYPE)
//        std::cout << "*";

    n->getName()->accept(this);

    if (n->getType()->getKind() == NK_ARRAY_TYPE)
    {
        std::cout << "[";
        // Print out type expression
        static_cast<ArrayTypeASTNode *>(n->getType())->getExpr()->accept(this);
        std::cout << "]";
    }

    if (n->getInit() != NULL_AST_NODE)
    {
        std::cout << " = ";
        n->getInit()->accept(this);
    }

    // }
}

void GenCVisitor::visit(ParmDeclASTNode *n)
{
    if (n->getType() != NULL_AST_NODE)
        n->getType()->accept(this);

    std::cout << " ";

//    if (n->getType()->getKind() == NK_POINTER_TYPE)
//        std::cout << "*";

    if (n->getName() != NULL_AST_NODE)
        n->getName()->accept(this);
}

void GenCVisitor::visit(FieldDeclASTNode *n)
{
//    _level++;
//    printTab(_level);
//    std::cout << "NK_FIELD_DECL" << std::endl;
//    if (n->getName() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Name:" << std::endl;
//        n->getName()->accept(this);
//    }
//    if (n->getType() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Type:" << std::endl;
//        n->getType()->accept(this);
//    }
//    _level--;

    if (n->getType() != NULL_AST_NODE)
        n->getType()->accept(this);

    std::cout << " ";

    if (n->getName() != NULL_AST_NODE)
        n->getName()->accept(this);

}

void GenCVisitor::visit(AsmStmtASTNode *n)
{
    printTab(_level + 1);
    std::cout << "NK_ASM_STMT" << std::endl;
    printTab(_level + 1);
    std::cout << "Data:" << std::endl;
    printTab(_level + 2);
    std::cout << n->getData() << std::endl;
}

void GenCVisitor::visit(BreakStmtASTNode *n)
{
    std::cout << "break";
}

void GenCVisitor::visit(CaseLabelASTNode *n)
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

void GenCVisitor::visit(CompoundStmtASTNode *n)
{
    printTab(_level);
    std::cout << "{" << std::endl;

    _level++;

    if (n->getDecls() != NULL_AST_NODE)
    {
        n->getDecls()->accept(this);
    }

    if (n->getStmts() != NULL_AST_NODE)
    {
        n->getStmts()->accept(this);
    }

    _level--;

    printTab(_level);
    std::cout << "}" << std::endl;
}

void GenCVisitor::visit(ContinueStmtASTNode *n)
{
    std::cout << "continue";
}

void GenCVisitor::visit(DoStmtASTNode *n)
{
    std::cout << "do" << std::endl;
    n->getBody()->accept(this);
    printTab(_level);
    std::cout << "while (";
    n->getCondition()->accept(this);
    std::cout << ");" << std::endl;
}

void GenCVisitor::visit(ForStmtASTNode *n)
{
    std::cout << "for (";
    if (n->getInit() != NULL_AST_NODE)
        n->getInit()->accept(this);

    std::cout << ";";
    if (n->getCondition() != NULL_AST_NODE)
    {
        std::cout << " ";
        n->getCondition()->accept(this);
    }
    std::cout << ";";
    if (n->getStep() != NULL_AST_NODE)
    {
        std::cout << " ";
        n->getStep()->accept(this);
    }

    std::cout << ")" << std::endl;
    n->getBody()->accept(this);
}

void GenCVisitor::visit(GotoStmtASTNode *n)
{
    std::cout << "goto ";
    n->getLabel()->accept(this);
}

void GenCVisitor::visit(IfStmtASTNode *n)
{
    std::cout << "if (";
    if (n->getCondition() != NULL_AST_NODE)
        n->getCondition()->accept(this);
    std::cout << ")" << std::endl;

    if (n->getThenClause() != NULL_AST_NODE)
    {
        if (n->getThenClause()->getKind() != NK_COMPOUND_STMT)
            printTab(_level+1);
        n->getThenClause()->accept(this);
    }

    if (n->getElseClause() != NULL_AST_NODE)
    {
        printTab(_level);
        std::cout << "else";
        if (n->getElseClause()->getKind() == NK_IF_STMT)
            std::cout << " ";
        else
            std::cout << std::endl;
        if (n->getElseClause()->getKind() != NK_COMPOUND_STMT &&
            n->getElseClause()->getKind() != NK_IF_STMT)
            printTab(_level+1);
        n->getElseClause()->accept(this);
    }
    std::cout << std::endl;
}

void GenCVisitor::visit(LabelStmtASTNode *n)
{
    int prevLevel = _level;
    _level = 0;
    n->getLabel()->accept(this);
    _level = prevLevel;
    std::cout << ":" << std::endl;
    printTab(_level);
    n->getStmt()->accept(this);
}

void GenCVisitor::visit(ReturnStmtASTNode *n)
{
    std::cout << "return";
    if (n->getExpr() != NULL_AST_NODE)
    {
        std::cout << " ";
        n->getExpr()->accept(this);
    }
}

void GenCVisitor::visit(SwitchStmtASTNode *n)
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

void GenCVisitor::visit(WhileStmtASTNode *n)
{
    std::cout << "while (";
    if (n->getCondition() != NULL_AST_NODE)
        n->getCondition()->accept(this);
    std::cout << ")" << std::endl;

    if (n->getBody() != NULL_AST_NODE)
        n->getBody()->accept(this);
}

void GenCVisitor::visit(CastExprASTNode *n)
{
//    _level++;
//    printTab(_level);
//    std::cout << "NK_CAST_EXPR" << std::endl;
//    if (n->getExpr() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Expression:" << std::endl;
//        n->getExpr()->accept(this);
//    }
//    if (n->getType() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Type:" << std::endl;
//        n->getType()->accept(this);
//    }
//    _level--;

    std::cout << "(";
    n->getType()->accept(this);
    std::cout << ") ";
    n->getExpr()->accept(this);
}

void GenCVisitor::visit(BitNotExprASTNode *n)
{
    std::cout << "~";
    n->getExpr()->accept(this);
}

void GenCVisitor::visit(LogNotExprASTNode *n)
{
    std::cout << "!";
    n->getExpr()->accept(this);
}

void GenCVisitor::visit(PredecrementExprASTNode *n)
{
    std::cout << "--";
    n->getExpr()->accept(this);
}

void GenCVisitor::visit(PreincrementExprASTNode *n)
{
    std::cout << "++";
    n->getExpr()->accept(this);
}

void GenCVisitor::visit(PostdecrementExprASTNode *n)
{
    n->getExpr()->accept(this);
    std::cout << "--";
}

void GenCVisitor::visit(PostincrementExprASTNode *n)
{
    n->getExpr()->accept(this);
    std::cout << "++";
}

void GenCVisitor::visit(AddrExprASTNode *n)
{
    std::cout << "&";
    n->getExpr()->accept(this);
}

void GenCVisitor::visit(IndirectRefASTNode *n)
{
    if (n->getField() == NULL_AST_NODE)
        std::cout << "*";

    if (n->getExpr() != NULL_AST_NODE)
        n->getExpr()->accept(this);

    if (n->getField() != NULL_AST_NODE)
    {
        std::cout << "->";
        n->getField()->accept(this);
    }

//    if (n->getType() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Type:" << std::endl;
//        n->getType()->accept(this);
//    }
//    _level--;
}

void GenCVisitor::visit(NopExprASTNode *n)
{
    _level++;
    printTab(_level);
    std::cout << "NK_NOP_EXPR" << std::endl;
    _level--;
}

void GenCVisitor::visit(LShiftExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " << ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(RShiftExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " >> ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(BitIorExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " | ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(BitXorExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " ^ ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(BitAndExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " & ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(LogAndExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " && ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(LogOrExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " || ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(PlusExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " + ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(MinusExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " - ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(MultExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " * ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(TruncDivExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " / ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(TruncModExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " % ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(ArrayRefASTNode *n)
{
//    _level++;
//    printTab(_level);
//    std::cout << "ARRAY_REF" << std::endl;
//    if (n->getExpr() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Expression:" << std::endl;
//        n->getExpr()->accept(this);
//    }
//    if (n->getIndex() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Index:" << std::endl;
//        n->getIndex()->accept(this);
//    }
//    if (n->getType() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Element type:" << std::endl;
//        n->getType()->accept(this);
//    }
//    _level--;

    if (n->getExpr() != NULL_AST_NODE)
        n->getExpr()->accept(this);

    std::cout << "[";

    if (n->getIndex() != NULL_AST_NODE)
        n->getIndex()->accept(this);

    std::cout << "]";
}

void GenCVisitor::visit(StructRefASTNode *n)
{
//    _level++;
//    printTab(_level);
//    std::cout << "STRUCT_REF" << std::endl;
//    if (n->getName() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Name:" << std::endl;
//        n->getName()->accept(this);
//    }
//    if (n->getMember() != NULL_AST_NODE)
//    {
//        printTab(_level);
//        std::cout << "Member:" << std::endl;
//        n->getMember()->accept(this);
//    }
//    _level--;

    if (n->getName() != NULL_AST_NODE)
        n->getName()->accept(this);

    std::cout << ".";

    if (n->getMember() != NULL_AST_NODE)
        n->getMember()->accept(this);
}

void GenCVisitor::visit(LtExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " < ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(LeExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " <= ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(GtExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " > ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(GeExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " >= ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(EqExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " == ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(NeExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " != ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(AssignExprASTNode *n)
{
    n->getLhs()->accept(this);
    std::cout << " = ";
    n->getRhs()->accept(this);
}

void GenCVisitor::visit(CondExprASTNode *n)
{
    n->getCondition()->accept(this);
    std::cout << " ? ";
    n->getThenClause()->accept(this);
    std::cout << " : ";
    n->getElseClause()->accept(this);
}

void GenCVisitor::visit(CallExprASTNode *n)
{
    n->getExpr()->accept(this);

    std::cout << "(";

    _inList++;
    n->getArgs()->accept(this);
    _inList--;

    std::cout << ")";
}

void GenCVisitor::visit(VoidTypeASTNode *n)
{
    std::cout << "void";
}

void GenCVisitor::visit(IntegralTypeASTNode *n)
{
    if (!n->getIsSigned())
        std::cout << "unsigned ";

    switch (n->getAlignment())
    {
        case 1:
            std::cout << "char";
            break;
        case 2:
            std::cout << "short";
            break;
        case 4:
            std::cout << "int";
            break;
        case 8:
            std::cout << "long";
            break;
        default:
            std::cout << "int";
            break;
    }
}

void GenCVisitor::visit(RealTypeASTNode *n)
{
    if (n->getIsDouble())
        std::cout << "double";
    else
        std::cout << "float";
}

void GenCVisitor::visit(EnumeralTypeASTNode *n)
{
    std::cout << "enum ";
    n->getName()->accept(this);
    printTab(_level);
    std::cout << std::endl << "{" << std::endl;
    _level++;
    _inList++;
    n->getBody()->accept(this);
    _inList--;
    _level--;
    printTab(_level);
    std::cout << "}";
}

void GenCVisitor::visit(PointerTypeASTNode *n)
{
//    std::cout << "* ";
    n->getBaseType()->accept(this);
    std::cout << "*";
}

void GenCVisitor::visit(FunctionTypeASTNode *n)
{
    // n->getType()->accept(this);
    // std::cout << "(*";
    // // Name
    // std::cout << ")";
    std::cout << "(";
    _inList++;
    n->getPrms()->accept(this);
    _inList--;
    std::cout << ")";
}

void GenCVisitor::visit(ArrayTypeASTNode *n)
{
    // std::cout << "[";
    // n->getExpr()->accept(this);
    // std::cout << "]" << std::endl;
    n->getElementType()->accept(this);
}

void GenCVisitor::visit(StructTypeASTNode *n)
{
    std::cout << "struct ";

    if (n->getName() != NULL_AST_NODE)
        n->getName()->accept(this);

    if (n->getBody() != NULL_AST_NODE)
    {
        std::cout << std::endl;
        printTab(_level);
        std::cout << "{" << std::endl;

        _level++;
        n->getBody()->accept(this);
        _level--;

        printTab(_level);
        std::cout << "}";
    }
}

void GenCVisitor::visit(UnionTypeASTNode *n)
{
    // TODO: Implement
}

void GenCVisitor::visit(NullASTNode *n)
{
    // Print nothing
}

static bool isNonSemi(ASTNodeKind n)
{
    return n == NK_IF_STMT || n == NK_WHILE_STMT || n == NK_DO_STMT ||
           n == NK_FUNCTION_DECL || n == NK_COMPOUND_STMT ||
           n == NK_INTEGRAL_TYPE;
}

void GenCVisitor::visit(SequenceASTNode *n)
{
    std::vector<ASTNode *> &elements = n->getElements();

    if (elements.size() > 0)
    {
        if (!_inList)
            printTab(_level);
        elements[0]->accept(this);
        if (!_inList)
        {
            if (!isNonSemi(elements[0]->getKind()))
                std::cout << ";" << std::endl;
        }
    }

    for (unsigned i = 1; i < elements.size(); i++)
    {
        if (elements[i] == NULL_AST_NODE)
            continue;

        if (!_inList)
            printTab(_level);
        else
            std::cout << ", ";
        elements[i]->accept(this);
        if (!_inList)
        {
            if (!isNonSemi(elements[i]->getKind()))
                std::cout << ";" << std::endl;
        }
    }
}

} // namespace cparser
