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
MalNode EVAL(MalNode ast, [[ maybe_unused ]] Environment& env) {
    switch (ast->type_) {
        case MalType::NodeType::List: {
            auto list_node = static_cast<List*>(ast.get());
            auto& children = list_node->children_;

            if (children.size() == 0)
                return ast;

            auto symbol = static_cast<Symbol*>(children[0].get());
            auto func = static_cast<Function*>(env.Get(symbol->symbol_).get());

            std::vector<MalNode> eval_children;
            for (auto& child : children | std::views::drop(1)) {
                eval_children.push_back(EVAL(child, env));
            }

            return func->ApplyFn(eval_children);
        }
        case MalType::NodeType::Symbol: {
            auto symbol_node = static_cast<Symbol*>(ast.get());

            auto& symbol = symbol_node->symbol_;

            return env.Get(symbol);
        }
        case MalType::NodeType::Vector: {
            auto vector_node = static_cast<Vector*>(ast.get());

            for(auto& child : vector_node->children_)
                child = EVAL(child, env);

            return ast;
        }
        case MalType::NodeType::HashMap: {
            auto hash_map_node = static_cast<HashMap*>(ast.get());

            for (auto& [k, child_node] : hash_map_node->kv_)
                child_node = EVAL(child_node, env);

            return ast;
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

std::string rep(std::string line, Environment env) {
    return PRINT(EVAL(READ(line), env));
}

/*
 * @brief Instantiates environment type with core functions
 *
 * */
void InitEnvironment(Environment& env) {
    env.Set("+", std::make_shared<Function>([](auto& nodes) -> MalNode {
        auto int_node = std::make_shared<Int>(0);
        for (auto& node : nodes)
            int_node->num_ += static_cast<Int*>(node.get())->num_;
        return int_node;
    }));

    env.Set("-", std::make_shared<Function>([](auto& nodes) -> MalNode {
        auto first_node_val = static_cast<Int*>(nodes[0].get())->num_;
        auto int_node = std::make_shared<Int>(first_node_val);
        for (auto& node : nodes | std::views::drop(1))
            int_node->num_ -= static_cast<Int*>(node.get())->num_;
        return int_node;
    }));

    env.Set("*", std::make_shared<Function>([](auto& nodes) -> MalNode {
        auto int_node = std::make_shared<Int>(1);
        for (auto& node : nodes)
            int_node->num_ *= static_cast<Int*>(node.get())->num_;
        return int_node;
    }));

    env.Set("/", std::make_shared<Function>([](auto& nodes) -> MalNode {
        auto first_node_val = static_cast<Int*>(nodes[0].get())->num_;
        auto int_node = std::make_shared<Int>(first_node_val);
        for (auto& node : nodes | std::views::drop(1))
            int_node->num_ /= static_cast<Int*>(node.get())->num_;
        return int_node;
    }));
}

int main() {
    Environment env;
    InitEnvironment(env);

    while (true) {
        std::cout << "user> ";

        if (std::cin.eof())
            std::exit(1);

        std::string line;
        std::getline(std::cin, line);

        try {
            std::cout << rep(line, env) << std::endl;
        } catch( const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}
