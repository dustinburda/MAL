//
// Created by Dustin on 7/6/25.
//

#ifndef MAL_ENVIRONMENT_H
#define MAL_ENVIRONMENT_H

#include <functional>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "types.h"

using EnvType = std::unordered_map<std::string, std::function<MalNode(std::vector<MalNode>&)>>;

std::unordered_map<std::string, std::function<MalNode(std::vector<MalNode>&)>> environment = {
        {"+", [](auto& nodes) -> MalNode {
            auto int_node = std::make_shared<Int>(0);
            for (auto& node : nodes)
                int_node->num_ += static_cast<Int*>(node.get())->num_;
            return int_node;
        }},
        {"-", [](auto& nodes) -> MalNode {
            auto first_node_val = static_cast<Int*>(nodes[0].get())->num_;
            auto int_node = std::make_shared<Int>(first_node_val);
            for (auto& node : nodes | std::views::drop(1))
                int_node->num_ -= static_cast<Int*>(node.get())->num_;
            return int_node;
        }},
        {"*", [](auto& nodes) -> MalNode {
            auto int_node = std::make_shared<Int>(1);
            for (auto& node : nodes)
                int_node->num_ *= static_cast<Int*>(node.get())->num_;
            return int_node;
        }},
        {"/", [](auto& nodes) -> MalNode {
            auto first_node_val = static_cast<Int*>(nodes[0].get())->num_;
            auto int_node = std::make_shared<Int>(first_node_val);
            for (auto& node : nodes | std::views::drop(1))
                int_node->num_ /= static_cast<Int*>(node.get())->num_;
            return int_node;
        }}
};

#endif //MAL_ENVIRONMENT_H
