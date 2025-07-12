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

class Environment {
public:
    Environment();
    Environment(Environment* outer);

    void Set(std::string symbol, MalNode data);
    MalNode Get(std::string key);
private:
    std::unordered_map<std::string, MalNode> map_;
    Environment* outer_;
};

#endif //MAL_ENVIRONMENT_H
