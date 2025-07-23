#include "../include/core.h"

auto plus =  std::make_shared<Function>([](auto& nodes) -> MalNode {
    auto int_node = std::make_shared<Int>(0);
    for (auto& node : nodes)
        int_node->num_ += static_cast<Int*>(node.get())->num_;
    return int_node;
});

auto subtract = std::make_shared<Function>([](auto& nodes) -> MalNode {
    auto first_node_val = static_cast<Int*>(nodes[0].get())->num_;
    auto int_node = std::make_shared<Int>(first_node_val);
    for (auto& node : nodes | std::views::drop(1))
        int_node->num_ -= static_cast<Int*>(node.get())->num_;
    return int_node;
});

auto multiply = std::make_shared<Function>([](auto& nodes) -> MalNode {
    auto int_node = std::make_shared<Int>(1);
    for (auto& node : nodes)
        int_node->num_ *= static_cast<Int*>(node.get())->num_;
    return int_node;
});

auto divide = std::make_shared<Function>([](auto& nodes) -> MalNode {
    auto first_node_val = static_cast<Int*>(nodes[0].get())->num_;
    auto int_node = std::make_shared<Int>(first_node_val);
    for (auto& node : nodes | std::views::drop(1))
        int_node->num_ /= static_cast<Int*>(node.get())->num_;
    return int_node;
});

auto list = std::make_shared<Function>([](auto& nodes) -> MalNode {
    auto list_node = std::make_shared<List>();
    list_node->children_ = nodes;

    return list_node;
});

auto is_list = std::make_shared<Function>([](auto& nodes) -> MalNode {
    auto list_node = dynamic_cast<List*>(nodes[0].get());

    return std::make_shared<Boolean>(list_node != nullptr);

});

auto is_empty =  std::make_shared<Function>([](auto& nodes) -> MalNode {
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
});

auto count = std::make_shared<Function>([](auto& nodes) -> MalNode {
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
});

auto less =  std::make_shared<Function>([](auto& nodes) -> MalNode {
    if (nodes.size() != 2)
        throw std::logic_error("< is a binary operator!");

    auto num1 = (nodes[0]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[0].get())->num_ : static_cast<Int*>(nodes[0].get())->num_;
    auto num2 = (nodes[1]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[1].get())->num_ : static_cast<Int*>(nodes[1].get())->num_;

    return std::make_shared<Boolean>(num1 < num2);
});

auto leq = std::make_shared<Function>([](auto& nodes) -> MalNode {
    if (nodes.size() != 2)
        throw std::logic_error("<= is a binary operator!");

    auto num1 = (nodes[0]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[0].get())->num_ : static_cast<Int*>(nodes[0].get())->num_;
    auto num2 = (nodes[1]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[1].get())->num_ : static_cast<Int*>(nodes[1].get())->num_;

    return std::make_shared<Boolean>(num1 <= num2);
});

auto greater = std::make_shared<Function>([](auto& nodes) -> MalNode {
    if (nodes.size() != 2)
        throw std::logic_error("> is a binary operator!");

    auto num1 = (nodes[0]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[0].get())->num_ : static_cast<Int*>(nodes[0].get())->num_;
    auto num2 = (nodes[1]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[1].get())->num_ : static_cast<Int*>(nodes[1].get())->num_;

    return std::make_shared<Boolean>(num1 > num2);

});

auto geq = std::make_shared<Function>([](auto& nodes) -> MalNode {
    if (nodes.size() != 2)
        throw std::logic_error(">= is a binary operator!");

    auto num1 = (nodes[0]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[0].get())->num_ : static_cast<Int*>(nodes[0].get())->num_;
    auto num2 = (nodes[1]->type_ == MalType::NodeType::Double)? static_cast<Double*>(nodes[1].get())->num_ : static_cast<Int*>(nodes[1].get())->num_;

    return std::make_shared<Boolean>(num1 >= num2);
});

auto equal = std::make_shared<Function>([](auto& nodes) -> MalNode {
    if (nodes.size() != 2)
        throw std::logic_error(">= is a binary operator!");

    auto equal = (*nodes[0] == *nodes[1]);

    return std::make_shared<Boolean>(equal);
});

auto prn =  std::make_shared<Function>([](auto& nodes) -> MalNode {
    for (std::size_t index = 0; auto& node : nodes) {
        std::cout << node->Print(true);

        if (nodes.size() > 1 && index < nodes.size() - 1)
            std::cout << " ";

        index++;
    }
    std::cout << "\n";

    return std::make_shared<Nil>();
});

auto pr_str = std::make_shared<Function>([](auto& nodes) -> MalNode {
    std::stringstream ss;
    for (std::size_t index = 0; auto& node : nodes) {
        ss << node->Print(true);

        if (nodes.size() > 1 && index < nodes.size() - 1)
            ss << " ";

        index++;
    }
    return std::make_shared<String>(ss.str());
});

auto str = std::make_shared<Function>([](auto& nodes) -> MalNode {
    std::stringstream ss;
    for (auto& node : nodes) {
        auto printed_value = node->Print(false);
        ss << printed_value;
    }
    return std::make_shared<String>(ss.str());
});

auto println =  std::make_shared<Function>([](auto& nodes) -> MalNode {
    std::stringstream ss;
    for (std::size_t index = 0; auto& node : nodes) {
        auto printed_value = node->Print(false);
        std::cout << printed_value;

        if (index != nodes.size() - 1)
            std::cout << " ";

        index++;
    }
    std::cout << "\n";
    return std::make_shared<Nil>();
});

Core::Core() {
    core_env_["+"] = plus;
    core_env_["-"] = subtract;
    core_env_["*"] = multiply;
    core_env_["/"] = divide;
    core_env_["list"] = list;
    core_env_["list?"] = is_list;
    core_env_["empty?"] = is_empty;
    core_env_["count"] = count;
    core_env_["<"] = less;
    core_env_["<="] = leq;
    core_env_[">"] = greater;
    core_env_[">="] = geq;
    core_env_["="] = equal;
    core_env_["prn"] = prn;
    core_env_["pr-str"] = pr_str;
    core_env_["str"] = str;
    core_env_["println"] = println;
}

const std::unordered_map<std::string, MalNode>& Core::GetEnv() const {
    return core_env_;
}