// C parser - header file.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#ifndef CPARSER_H
#define CPARSER_H

#include <map>

#include "AbstractSyntaxTree.h"
#include "Parser.h"
#include "Lexer.h"
#include "CLexer.h"

namespace cparser
{

class CParser : public Parser
{
    bool _inTypedef;

    void initNames();

    bool isTypeSpecifier(unsigned _kind, int n);
    bool isTypeQualifier(unsigned kind);
    bool isStorageClassSpecifier(unsigned kind);

    // THE PARSER RULES

    // Expressions
    ASTNode *PrimaryExpression();
    ASTNode *GenericSelection();
    ASTNode *GenericAssocList();
    ASTNode *GenericAssociation();
    ASTNode *parsePostfixExpression(ASTNode *expr);
    ASTNode *PostfixExpression(ASTNode *typeName);
    ASTNode *ArgumentExpressionList();
    ASTNode *UnaryExpression(ASTNode *typeName);
    ASTNodeKind UnaryOperator();
    ASTNode *CastExpression();
    ASTNode *MultiplicativeExpression();
    ASTNode *AdditiveExpression();
    ASTNode *ShiftExpression();
    ASTNode *RelationalExpression();
    ASTNode *EqualityExpression();
    ASTNode *AndExpression();
    ASTNode *ExclusiveOrExpression();
    ASTNode *InclusiveOrExpression();
    ASTNode *LogicalAndExpression();
    ASTNode *LogicalOrExpression();
    ASTNode *ConditionalExpression();
    ASTNode *AssignmentExpression();
    ASTNode *Expression();
    ASTNode *ConstantExpression();

    // Declarations
    ASTNode *Declaration(ASTNode *declSpec);
    ASTNode *DeclarationSpecifiers();
    SequenceASTNode *InitDeclaratorList(ASTNode *typeSpec);
    ASTNode *InitDeclarator(ASTNode *typeSpec);
    void StorageClassSpecifier(unsigned &flags);
    ASTNode *TypeSpecifier();
    ASTNode *StructOrUnionSpecifier();
    SequenceASTNode *StructDeclarationList();
    ASTNode *StructDeclaration();
    ASTNode *SpecifierQualifierList();
    SequenceASTNode *StructDeclaratorList(ASTNode *typeSpec);
    ASTNode *StructDeclarator(ASTNode *typeSpec);
    ASTNode *EnumSpecifier();
    SequenceASTNode *EnumeratorList();
    void TypeQualifier(unsigned &flags);
    void FunctionSpecifier();
    ASTNode *AlignmentSpecifier();
    ASTNode *Declarator(ASTNode * &typeSpec);
    ASTNode *DirectDeclarator(ASTNode * &typeSpec);
    ASTNode *GccDeclaratorExtension();
    ASTNode *GccAttributeSpecifier();
    ASTNode *GccAttributeList();
    ASTNode *GccAttribute();
    ASTNode *ParameterTypeList();
    ASTNode *ParameterList();
    ASTNode *ParameterDeclaration();
    ASTNode *IdentifierList();
    ASTNode *TypeName();
    ASTNode *AbstractDeclarator();
    ASTNode *DirectAbstractDeclarator();
    ASTNode *Initializer();
    ASTNode *InitializerList();

    // Statements
    ASTNode *Statement();
    ASTNode *LabeledStatement();
    ASTNode *CompoundStatement();
    ASTNode *FunctionBody();
    ASTNode *DeclarationList();
    ASTNode *StmtOrDeclList();
    ASTNode *SelectionStatement();
    ASTNode *IterationStatement();
    ASTNode *JumpStatement();
    ASTNode *ExpressionStatement();
    ASTNode *TranslationUnit();
    ASTNode *ExternalDeclaration();
    ASTNode *FunctionDefinition(ASTNode *funcType);
public:
    void parse(const char *output);

    CParser(CLexer *LEX) : Parser(LEX)
    {
        _inTypedef = false;
        initNames();
    }
};

} // namespace cparser

#endif
