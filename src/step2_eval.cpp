#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

#include "../include/environment.h"
#include "../include/printer.h"
#include "../include/reader.h"

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
MalNode EVAL(MalNode ast, [[ maybe_unused ]] EnvType& env) {
    switch (ast->type_) {
        case MalType::NodeType::List: {
            auto list_node = static_cast<List*>(ast.get());
            auto& children = list_node->children_;

            if (children.size() == 0)
                return ast;

            auto symbol = static_cast<Symbol*>(children[0].get());
            auto func = env[symbol->symbol_];

            std::vector<MalNode> eval_children;
            for (auto& child : children | std::views::drop(1)) {
                eval_children.push_back(EVAL(child, env));
            }

            return func(eval_children);
        }
        case MalType::NodeType::Symbol: {
            auto symbol_node = static_cast<Symbol*>(ast.get());

            auto& symbol = symbol_node->symbol_;

            if (env.count(symbol) == 0)
                throw std::logic_error ("Symbol not in environment!");



        }
        default:
            break;
    }

    return ast;
}

/*
 * @brief Takes an expression and prints it
 *
 * @param
 * */
std::string PRINT(MalNode ast) {
    return PrintAst(ast);
}

std::string rep(std::string line) {
    return PRINT(EVAL(READ(line), environment));
}

int main() {
    while (true) {
        std::cout << "user> ";

        if (std::cin.eof())
            std::exit(1);

        std::string line;
        std::getline(std::cin, line);

        try {
            std::cout << rep(line) << std::endl;
        } catch( const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}
