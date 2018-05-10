OBJS = cformat.o CParser.o Parser.o SymbolTable.o CLexer.o Lexer.o \
        AbstractSyntaxTree.o ASTNode.o GenCVisitor.o PrintTreeVisitor.o \
        TreeVisitor.o

CXX = g++
CXXFLAGS = -std=c++14 -Wall -g

SRC = src
INCLUDE = include
BIN = .
DEST = /usr/bin

all: cformat

cformat: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o cformat

cformat.o: ${SRC}/cformat.cpp ${INCLUDE}/CParser.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/cformat.cpp

Parser.o: ${INCLUDE}/Parser.h ${INCLUDE}/Lexer.h ${INCLUDE}/SymbolTable.h \
 ${INCLUDE}/AbstractSyntaxTree.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/Parser.cpp

CParser.o: ${INCLUDE}/CParser.h ${INCLUDE}/Lexer.h ${INCLUDE}/SymbolTable.h \
 ${INCLUDE}/AbstractSyntaxTree.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/CParser.cpp

SymbolTable.o: ${INCLUDE}/SymbolTable.h ${INCLUDE}/common.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/SymbolTable.cpp

Lexer.o: ${INCLUDE}/Lexer.h ${INCLUDE}/common.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/Lexer.cpp

CLexer.o: ${INCLUDE}/CLexer.h ${INCLUDE}/common.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/CLexer.cpp

AbstractSyntaxTree.o: ${INCLUDE}/AbstractSyntaxTree.h \
 ${INCLUDE}/ASTNode.h ${INCLUDE}/TreeVisitor.h ${INCLUDE}/PrintTreeVisitor.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/AbstractSyntaxTree.cpp

PrintTreeVisitor.o: ${INCLUDE}/ASTNode.h ${INCLUDE}/TreeVisitor.h \
 ${INCLUDE}/PrintTreeVisitor.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/PrintTreeVisitor.cpp

GenCVisitor.o: ${INCLUDE}/ASTNode.h ${INCLUDE}/TreeVisitor.h \
 ${INCLUDE}/GenCVisitor.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/GenCVisitor.cpp

TreeVisitor.o: ${INCLUDE}/ASTNode.h ${INCLUDE}/TreeVisitor.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/TreeVisitor.cpp

ASTNode.o: ${INCLUDE}/TreeVisitor.h ${INCLUDE}/ASTNode.h
	$(CXX) $(CXXFLAGS) -c ${SRC}/ASTNode.cpp

install:
	-cp ${BIN}/cformat ${DEST}

clean:
	-rm *.o cformat typechecker
