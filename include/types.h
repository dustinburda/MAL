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
        Quote,
        Quasiquote,
        Unquote,
        Nil
    };

    MalType(NodeType type) : type_{type} {}
    virtual ~MalType() {}
    virtual std::string Print() = 0;

    MalType::NodeType type_;
};



using MalNode = std::shared_ptr<MalType>;

struct List : MalType {
    List() : MalType{NodeType::List}, children_{} {}
    ~List() override  {}

    void Add(MalNode node) {
        children_.push_back(node);
    }

    std::string Print() override {
        std::stringstream ss;

        ss << "(";
        for (size_t index = 0;auto& child : children_) {
            ss << child->Print();

            index++;

            if (index != children_.size())
                ss << " ";
        }
        ss << ")";

        return ss.str();
    }

    std::vector<MalNode> children_;
};

struct Vector : MalType {
    Vector() : MalType{NodeType::Vector}, children_{} {}
    ~Vector() override { }

    void Add(MalNode node) {
        children_.push_back(node);
    }

    std::string Print() override {
        std::stringstream ss;

        ss << "[";
        for (size_t index = 0;auto& child : children_) {
            ss << child->Print();

            index++;

            if (index != children_.size())
                ss << " ";
        }
        ss << "]";

        return ss.str();
    }

    std::vector<MalNode> children_;
};

struct HashMap : MalType {
    HashMap() : MalType{NodeType::HashMap}, kv_{} {}
    ~HashMap() override  {}

    std::string Print() override {
        std::stringstream ss;

        ss << "{";
        for (size_t index = 0; auto& [k, v] : kv_) {
            ss << k << " " <<  v->Print();

            index++;

            if (index != kv_.size())
                ss << " ";
        }
        ss << "}";

        return ss.str();
    }

    std::unordered_map<std::string, MalNode> kv_;
};

struct Int : MalType {
    explicit Int(int i) : MalType{NodeType::Int}, num_{i} {}
    ~Int() override { }

    std::string Print() override {
        std::stringstream ss;

        ss << num_;

        return ss.str();
    }

    int num_;
};

struct Double : MalType {
    explicit Double(double d) : MalType{NodeType::Double}, num_{d} {}
    ~Double() override {}

    std::string Print() override {
        std::stringstream ss;

        ss << num_;

        return ss.str();
    }

    double num_;
};

struct Keyword : MalType {
    explicit Keyword(std::string keyword) : MalType(NodeType::Keyword), keyword_{keyword.substr(1, keyword.size() - 1)} {}
    ~Keyword() override {}

    std::string Print() override {
        std::stringstream ss;

        ss << ":";
        ss << keyword_;

        return ss.str();
    }

    std::string keyword_;
};

struct String : MalType {
    explicit String(std::string s) : MalType{NodeType::String}, s_{s} {}
    ~String() override {}

    std::string Print() override {
        std::stringstream ss;
        ss << "\"";
        ss << s_;
        ss << "\"";

        return ss.str();
    }

    std::string s_;
};

struct Boolean : MalType {
    explicit Boolean(bool b) : MalType{NodeType::Boolean}, b_{b} {}
    ~Boolean() override {}

    std::string Print() override {
        std::stringstream ss;

        ss << ((b_) ? "true" : "false");

        return ss.str();
    }

    bool b_;
};

struct Symbol : MalType {
    explicit Symbol(char symbol) : MalType{NodeType::Symbol}, symbol_{symbol} {}
    explicit Symbol(std::string symbol) : MalType{NodeType::Symbol}, symbol_{symbol} { }
    ~Symbol() override {}

    std::string Print() override {
        std::stringstream ss;

        ss << symbol_;

        return ss.str();
    }

    std::string symbol_;
};

struct Quote : MalType {
    explicit Quote(MalNode child) : MalType{NodeType::Quote}, child_{child} {}
    ~Quote() override {}

    std::string Print() override {
        std::stringstream ss;

        ss << "(quote ";
        ss << child_->Print();
        ss << ")";

        return ss.str();
    }

    MalNode child_;
};

struct Quasiquote : MalType {
    explicit Quasiquote(MalNode child) : MalType{NodeType::Quasiquote}, child_{child} {}
    ~Quasiquote() override {}

    std::string Print() override {
        std::stringstream ss;

        ss << "(quasiquote ";
        ss << child_->Print();
        ss << ")";

        return ss.str();
    }

    MalNode child_;
};

struct Unquote : MalType {
    explicit Unquote(MalNode child) : MalType{NodeType::Unquote}, child_{child} {}
    ~Unquote() override {}

    std::string Print() override {
        std::stringstream ss;

        ss << "(unquote ";
        ss << child_->Print();
        ss << ")";

        return ss.str();
    }

    MalNode child_;
};


struct Nil : MalType {
    Nil() : MalType{NodeType::Nil} {}
    ~Nil() override {}

    std::string Print() override {
        std::stringstream ss;

        ss << "nil";

        return ss.str();
    }
};



#endif // MAL_TYPES_H