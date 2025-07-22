#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

#include "../include/environment.h"
#include "../include/printer.h"
#include "../include/reader.h"
#include "../include/repfuncs.h"

std::vector<std::shared_ptr<Environment>> envs;

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

MalNode Apply(Environment& env, std::vector<MalNode>& children) {

    if (dynamic_cast<Symbol*>(children[0].get())) {
        auto& symbol = static_cast<Symbol*>(children[0].get())->symbol_;

        if (symbol == "def!") {
            auto variable = static_cast<Symbol*>(children[1].get())->symbol_;
            auto value = EVAL(children[2], env);

            env.Set(variable, value);

            return value;
        } else if (symbol == "let*") {
            auto current_env = std::make_shared<Environment>(&env);
            envs.push_back(current_env);

            auto binding_list = static_cast<List*>(children[1].get())->children_;

            for(std::size_t i = 0; i < binding_list.size() - 1; i += 2) {
                auto var = static_cast<Symbol*>(binding_list[i].get())->symbol_;
                auto val = EVAL(binding_list[i+1], *current_env);

                current_env->Set(var, val);
            }
            return EVAL(children[2], *current_env);

        } else if (symbol == "do") {
            MalNode ret = nullptr;
            for (std::size_t i = 1; i < children.size(); i++) {
                auto eval_child = EVAL(children[i], env);

                if ( i == children.size() - 1)
                    ret = eval_child;
            }

            return ret;
        } else if (symbol == "if") {
            assert(children.size() == 3 || children.size() == 4);

            auto eval_condition = EVAL(children[1], env);

            if ( (eval_condition->type_ == MalType::NodeType::Nil) ||
                 (eval_condition->type_ == MalType::NodeType::Boolean && !static_cast<Boolean*>(eval_condition.get())->b_))
                return (children.size() == 4) ? EVAL(children[3], env) : std::make_shared<Nil>();

            return EVAL(children[2], env);
        } else if (symbol == "fn*") {
            auto fn_vars = static_cast<List*>(children[1].get());

            std::vector<std::string> binds;
            for (auto& var : fn_vars->children_) {
                auto var_symbol = static_cast<Symbol*>(var.get())->symbol_;
                binds.push_back(var_symbol);
            }

            auto fn_body = children[2];

            auto closure = std::make_shared<Function>([binds, fn_body, &env](std::vector<MalNode>& exprs) -> MalNode {
                auto new_env = std::make_shared<Environment>(&env, binds, exprs);
                envs.push_back(new_env);

                return EVAL(fn_body, *new_env);
            });

            return closure;
        }
    }

    auto node = EVAL(children[0], env);
    auto func = dynamic_cast<Function*>(node.get());
    std::vector<MalNode> eval_children;
    for (auto& child : children | std::views::drop(1))
        eval_children.emplace_back(EVAL(child, env));

    return func->ApplyFn(eval_children);
}

/*
 * @brief Takes an AST and Environment map and evaluates the AST
 *
 * @param AST and Environment
 * @return Parsed expression
 * */
MalNode EVAL(MalNode ast, Environment& env) {
    switch (ast->type_) {
        case MalType::NodeType::List: {
            auto list_node = static_cast<List*>(ast.get());

            auto& children = list_node->children_;
            if (children.size() == 0)
                return ast;

            auto node =  Apply(env, children);
            return node;
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
 * */
std::string PRINT(MalNode ast) {
    return PrintAst(ast);
}

std::string rep(std::string line, Environment& env) {
    return PRINT(EVAL(READ(line), env));
}

/*
 * @brief Instantiates environment type with core functions
 *
 * */
void InitEnvironment(Environment& env) {
    rep("(def! not (fn* (a) (if a false true)))", env);

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

    env.Set("list", std::make_shared<Function>([](auto& nodes) -> MalNode {
        auto list_node = std::make_shared<List>();
        list_node->children_ = nodes;

        return list_node;
    }));

    env.Set("list?", std::make_shared<Function>([](auto& nodes) -> MalNode {
        auto list_node = dynamic_cast<List*>(nodes[0].get());

        return std::make_shared<Boolean>(list_node != nullptr);

    }));

    env.Set("empty?", std::make_shared<Function>([](auto& nodes) -> MalNode {
        std::size_t size = 0;
        switch (nodes[0]->type_) {
            case MalType::NodeType::List:
                size = static_cast<List*>(nodes[0].get())->children_.size();
                break;
            case MalType::NodeType::Vector:
                size = static_cast<Vector*>(nodes[0].get())->children_.size();
                break;
            default:
                throw std::logic_error("First parameter must be a list or a vector!");
        }

        return std::make_shared<Boolean>(size == 0);
    }));

    env.Set("count", std::make_shared<Function>([](auto& nodes) -> MalNode {
        auto size = 0;
        switch (nodes[0]->type_) {
            case MalType::NodeType::Nil:
                size = 0;
                break;
            case MalType::NodeType::Vector:
                size = static_cast<Vector*>(nodes[0].get())->children_.size();
                break;
            case MalType::NodeType::List:
                size = static_cast<List*>(nodes[0].get())->children_.size();
                break;
            default:
                throw std::logic_error("count not found!");
        }
        return std::make_shared<Int>(size);
    }));

    env.Set("<", std::make_shared<Function>([](auto& nodes) -> MalNode {
        if (nodes.size() != 2)
            throw std::logic_error("< is a binary operator!");

        auto num1 = (nodes[0]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[0].get())->num_ : static_cast<Int*>(nodes[0].get())->num_;
        auto num2 = (nodes[1]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[1].get())->num_ : static_cast<Int*>(nodes[1].get())->num_;

        return std::make_shared<Boolean>(num1 < num2);
    }));

    env.Set("<=", std::make_shared<Function>([](auto& nodes) -> MalNode {
        if (nodes.size() != 2)
            throw std::logic_error("<= is a binary operator!");

        auto num1 = (nodes[0]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[0].get())->num_ : static_cast<Int*>(nodes[0].get())->num_;
        auto num2 = (nodes[1]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[1].get())->num_ : static_cast<Int*>(nodes[1].get())->num_;

        return std::make_shared<Boolean>(num1 <= num2);
    }));

    env.Set(">", std::make_shared<Function>([](auto& nodes) -> MalNode {
        if (nodes.size() != 2)
            throw std::logic_error("> is a binary operator!");

        auto num1 = (nodes[0]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[0].get())->num_ : static_cast<Int*>(nodes[0].get())->num_;
        auto num2 = (nodes[1]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[1].get())->num_ : static_cast<Int*>(nodes[1].get())->num_;

        return std::make_shared<Boolean>(num1 > num2);

    }));

    env.Set(">=", std::make_shared<Function>([](auto& nodes) -> MalNode {
        if (nodes.size() != 2)
            throw std::logic_error(">= is a binary operator!");

        auto num1 = (nodes[0]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[0].get())->num_ : static_cast<Int*>(nodes[0].get())->num_;
        auto num2 = (nodes[1]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[1].get())->num_ : static_cast<Int*>(nodes[1].get())->num_;

        return std::make_shared<Boolean>(num1 >= num2);
    }));

    env.Set("=", std::make_shared<Function>([](auto& nodes) -> MalNode {
        if (nodes.size() != 2)
            throw std::logic_error(">= is a binary operator!");

        auto equal = (*nodes[0] == *nodes[1]);

        return std::make_shared<Boolean>(equal);
    }));

    env.Set("prn", std::make_shared<Function>([](auto& nodes) -> MalNode {
        for (std::size_t index = 0; auto& node : nodes) {
            std::cout << node->Print(true);

            if (nodes.size() > 1 && index < nodes.size() - 1)
                std::cout << " ";

            index++;
        }
        std::cout << "\n";

        return std::make_shared<Nil>();
    }));

    env.Set("pr-str", std::make_shared<Function>([](auto& nodes) -> MalNode {
        std::stringstream ss;
        for (std::size_t index = 0; auto& node : nodes) {
            ss << node->Print(true);

            if (nodes.size() > 1 && index < nodes.size() - 1)
                ss << " ";

            index++;
        }
        return std::make_shared<String>(ss.str());
    }));

    env.Set("str", std::make_shared<Function>([](auto& nodes) -> MalNode {
        std::stringstream ss;
        for (auto& node : nodes) {
            auto printed_value = node->Print(false);
            if (node->type_ == MalType::NodeType::String)
                printed_value = printed_value.substr(1, printed_value.size() - 2);

            ss << printed_value;
        }
        return std::make_shared<String>(ss.str());
    }));

    env.Set("println", std::make_shared<Function>([](auto& nodes) -> MalNode {
        std::stringstream ss;
        for (std::size_t index = 0; auto& node : nodes) {
            auto printed_value = node->Print(false);
            if (node->type_ == MalType::NodeType::String)
                printed_value = printed_value.substr(1, printed_value.size() - 2);

            std::cout << printed_value;
            if (index != nodes.size() - 1)
                std::cout << " ";

            index++;
        }
        std::cout << "\n";
        return std::make_shared<Nil>();
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
