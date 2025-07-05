#ifndef MAL_TYPES_H
#define MAL_TYPES_H

#include <cassert>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

struct MalType {
    enum class NodeType {
        List,
        Vector,
        HashMap,
        Int,
        Double,
        Keyword,
        String,
        Boolean,
        Symbol,
        Nil
    };

    MalType(NodeType type) : type_{type} {}
    virtual std::string Print() = 0;

    MalType::NodeType type_;
};



using MalNode = std::shared_ptr<MalType>;

struct List : MalType {
    List() : MalType{NodeType::List}, children_{} {}

    void Add(MalNode node) {
        children_.push_back(node);
    }

    std::string Print() override {
        std::stringstream ss;

        for (size_t index = 0;auto& child : children_) {
            ss << child->Print();

            index++;

            if (index != children_.size())
                ss << " ";
        }

        return ss.str();
    }

    std::vector<MalNode> children_;
};

struct Vector : MalType {
    Vector() : MalType{NodeType::Vector}, children_{} {}

    void Add(MalNode node) {
        children_.push_back(node);
    }

    std::string Print() override {
        std::stringstream ss;

        for (auto& child : children_) {
            ss << child->Print();
        }

        return ss.str();
    }

    std::vector<MalNode> children_;
};

struct HashMap : MalType {
    HashMap() : MalType{NodeType::HashMap}, kv_{} {}

    std::unordered_map<std::string, MalNode> kv_;
};

struct Int : MalType {
    explicit Int(int i) : MalType{NodeType::Int}, num_{i} {}

    std::string Print() override {
        std::stringstream ss;

        ss << num_;

        return ss.str();
    }

    int num_;
};

struct Double : MalType {
    explicit Double(double d) : MalType{NodeType::Double}, num_{d} {}

    std::string Print() override {
        std::stringstream ss;

        ss << num_;

        return ss.str();
    }

    double num_;
};

struct Keyword : MalType {
    explicit Keyword(std::string keyword) : MalType(NodeType::Keyword), keyword_{keyword.substr(1, keyword.size() - 1)} {}

    std::string Print() override {
        std::stringstream ss;

        ss << keyword_;

        return ss.str();
    }

    std::string keyword_;
};

struct String : MalType {
    explicit String(std::string s) : MalType{NodeType::String}, s_{s} {}

    std::string Print() override {
        std::stringstream ss;

        ss << s_;

        return ss.str();
    }

    std::string s_;
};

struct Boolean : MalType {
    explicit Boolean(bool b) : MalType{NodeType::Boolean}, b_{b} {}

    std::string Print() override {
        std::stringstream ss;

        ss << ((b_) ? "true" : "false");

        return ss.str();
    }

    bool b_;
};

struct Symbol : MalType {
    explicit Symbol(char symbol) : MalType{NodeType::Symbol}, symbol_{symbol} {}
    explicit Symbol(std::string symbol) : MalType{NodeType::Symbol}, symbol_{symbol[0]} { assert(symbol.size() == 1); }

    std::string Print() override {
        std::stringstream ss;

        ss << symbol_;

        return ss.str();
    }

    char symbol_;
};

struct Nil : MalType {
    Nil() : MalType{NodeType::Nil} {}

    std::string Print() override {
        std::stringstream ss;

        ss << "nil";

        return ss.str();
    }
};



#endif // MAL_TYPES_H