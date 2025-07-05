#include <cstdlib>
#include <iostream>
#include <string>

#include "printer.h"
#include "reader.h"
#include "types.h"

/*
 * @brief Takes source code and tokenizes + parses it into an AST
 *
 * @param Source code
 * @return AST
 * */
MalNode ReadStr(std::string src) {
    Reader r {src};
    r.Tokenize();
    auto root = r.ReadForm();

    return root;
}

/*
 * @brief Takes a line of source and calls ReadStr
 *
 * @param Source code
 * @return AST
 * */
MalNode READ(std::string line) {
    return ReadStr(line);
}

/*
 * @brief Takes an AST and Environment map and evaluates the AST
 *
 * @param AST and Environment
 * @return Parsed expression
 * */
MalNode EVAL(MalNode ast, [[ maybe_unused ]] std::string env) {
    return ast;
}

/*
 * @brief Takes an expression and prints it
 *
 * @param
 * */
std::string PRINT(MalNode ast) {
    return print_ast(ast);
}

std::string rep(std::string line) {
    return PRINT(EVAL(READ(line), ""));
}

int main() {
    std::string line;

    while (true) {
        std::cout << "user> ";

        if (std::cin.eof())
            std::exit(1);

        std::string line;
        std::getline(std::cin, line);

        std::cout << rep(line) << std::endl;
    }


}
