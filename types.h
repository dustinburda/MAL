#ifndef MAL_TYPES_H
#define MAL_TYPES_H

#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>

struct MalType {
    enum class NodeType {
        List,
        Vector,
        HashMap,
        Int,
        Double,
        String,
        Boolean,
        Symbol,
        Nil
    };

    MalType::NodeType type_;
};



using MalNode = std::shared_ptr<MalType>;

struct List : MalType {
    List() : MalType{NodeType::List}, children_{} {}

    void Add(MalNode node) {
        children_.push_back(node);
    }

    std::vector<MalNode> children_;
};

struct Vector : MalType {
    Vector() : MalType{NodeType::Vector}, children_{} {}

    void Add(MalNode node) {
        children_.push_back(node);
    }

    std::vector<MalNode> children_;
};

struct HashMap : MalType {
    HashMap() : MalType{NodeType::HashMap}, kv_{} {}

    std::unordered_map<std::string, MalNode> kv_;
};

struct Int : MalType {
    explicit Int(int i) : MalType{NodeType::Int}, num_{i} {}
    int num_;
};

struct Double : MalType {
    explicit Double(double d) : MalType{NodeType::Double}, num_{d} {}
    double num_;
};

struct String : MalType {
    explicit String(std::string s) : MalType{NodeType::String}, s_{s} {}

    std::string s_;
};

struct Boolean : MalType {
    explicit Boolean(bool b) : MalType{NodeType::Boolean}, b_{b} {}
    bool b_;
};

struct Symbol : MalType {
    explicit Symbol(char symbol) : MalType{NodeType::Symbol}, symbol_{symbol} {}
    explicit Symbol(std::string symbol) : MalType{NodeType::Symbol}, symbol_{symbol[0]} { assert(symbol.size() == 1); }

    char symbol_;
};

class Nil : MalType {

};



#endif // MAL_TYPES_H