//
// Created by Dustin on 7/11/25.
//

#include "../include/environment.h"

Environment::Environment() : outer_{nullptr} {}

Environment::Environment(Environment* outer) : outer_{outer} {}

Environment::Environment(Environment* outer, std::vector<std::string>& bind, std::vector<MalNode>& exprs) : outer_{outer} {
    if (bind.size() != exprs.size())
        throw std::logic_error("Number of binds must be equal to the number of exprs");

    for (std::size_t index = 0; index < bind.size(); index++) {
        auto& symbol = bind[index];
        auto& expr = exprs[index];
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
