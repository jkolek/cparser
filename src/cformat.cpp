// cformat
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "../include/CLexer.h"
#include "../include/CParser.h"
#include "../include/PrintTreeVisitor.h"
#include "../include/GenCVisitor.h"

#define VERSION "0.1"

#define INFO_STR                                                               \
    "cformat " VERSION "\n"                                                    \
    "Copyright (C) 2017, 2018 Jozef Kolek  <jkolek@gmail.com>. "               \
    "All Rights Reserved.\n\n"

#define HELP_STR                                                               \
    "INPUT and OUTPUT stands for input and output files respectively\n\n"      \
    "  -o, --output             Output file\n"                                 \
    "  -h, --help               Print out this help information\n"             \
    "  -v, --version            Print out only version information\n\n"

void print_info() { std::cout << INFO_STR; }

void print_help() { std::cout << HELP_STR; }

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        print_info();
        std::cout << "Try -h option for more info." << std::endl;
        exit(1);
    }

    char *input = NULL;
    char output[256];
    // bool outputOk = false;
    bool printHelp = false;
    bool printVersion = false;
    int n = 1;

    while (n < argc)
    {
        if (strcmp(argv[n], "-o") == 0 || strcmp(argv[n], "--output") == 0)
        {
            strcpy(output, argv[++n]);
            // outputOk = true;
        }
        else if (strcmp(argv[n], "-h") == 0 || strcmp(argv[n], "--help") == 0)
        {
            printHelp = true;
        }
        else if (strcmp(argv[n], "-v") == 0 ||
                 strcmp(argv[n], "--version") == 0)
        {
            printVersion = true;
        }
        else
        {
            input = argv[n];
        }
        n++;
    }

    if (printHelp)
    {
        print_info();
        print_help();
        exit(0);
    }

    if (printVersion)
    {
        print_info();
        exit(0);
    }

    if (input == NULL)
    {
        std::cerr << "cformat: fatal error: no input file" << std::endl;
        exit(1);
    }

    cparser::CLexer lexer(input);
    cparser::CParser parser(&lexer);
    parser.parse(output);

    cparser::AbstractSyntaxTree *ast = parser.getAST();

    // std::cout << std::endl << "Abstract syntax tree:" << std::endl << std::endl;
    // cparser::TreeVisitor *visitor = new cparser::PrintTreeVisitor();
    // ast->visit(visitor);
    // std::cout << std::endl;

    // std::cout << "===============================================" << std::endl;

    cparser::TreeVisitor *genCVisitor = new cparser::GenCVisitor();
    ast->visit(genCVisitor);

    // delete visitor;
    delete genCVisitor;

    return 0;
}
