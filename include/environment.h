//
// Created by Dustin on 7/6/25.
//

#ifndef MAL_ENVIRONMENT_H
#define MAL_ENVIRONMENT_H

#include <functional>
#include <unordered_map>

using EnvType = std::unordered_map<char, std::function<int(int, int)>>;

std::unordered_map<char, std::function<int(int, int)>> environment = {
        {'+', [](auto... num) -> int { return (num + ...); }},
        {'-', [](auto... num) -> int { return (num - ...); }},
        {'*', [](auto... num) -> int { return (num * ...); }},
        {'/', [](auto... num) -> int { return (num / ...); }}
};

#endif //MAL_ENVIRONMENT_H
