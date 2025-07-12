//
// Created by Dustin on 7/11/25.
//

#include "../include/environment.h"

Environment::Environment() : outer_{nullptr} {}

Environment::Environment(Environment* outer) : outer_{outer} {}

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
