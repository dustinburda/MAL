//
// Created by Dustin on 7/11/25.
//

#include "../include/environment.h"

Environment::Environment() : outer_{nullptr} {}

Environment::Environment(Environment* outer) : outer_{outer} {}

Environment::Environment(Environment* outer, const std::vector<std::string>& bind, const std::vector<MalNode>& exprs) : outer_{outer} {
    for (std::size_t index = 0; index < bind.size(); index++) {
        auto& symbol = bind[index];
        auto& expr = exprs[index];

        if (symbol == "&")
        {
            auto more_symbol = bind[index + 1];

            auto remaining_args = std::make_shared<List>();
            remaining_args->children_ = std::vector<MalNode> {exprs.begin() + index, exprs.end()};

            map_[more_symbol] = remaining_args;
            return;
        }

        map_[symbol] = expr;
    }
}


void Environment::Set(std::string symbol, MalNode data) {
    map_[symbol] = data;
}

MalNode Environment::Get(std::string key) {
    if (map_.contains(key) == 0)
    {
        if (outer_ == nullptr)
            throw std::logic_error(key + " not found!");

        return outer_->Get(key);
    }

    return map_[key];
}
