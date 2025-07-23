#ifndef MAL_CORE_H
#define MAL_CORE_H

#include <iostream>
#include <memory>
#include <ranges>

#include "types.h"

struct Core {
public:
    Core();
    const std::unordered_map<std::string, MalNode>& GetEnv() const;

private:
    std::unordered_map<std::string, MalNode> core_env_;
};

#endif // MAL_CORE_H