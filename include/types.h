#ifndef MAL_TYPES_H
#define MAL_TYPES_H

#include <cassert>
#include <functional>
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
        Function,
        Nil
    };

    MalType(NodeType type) : type_{type} {}
    virtual ~MalType() {}
    virtual std::string Print(bool print_readably) = 0;
    virtual bool operator==(MalType& other) = 0;

    MalType::NodeType type_;
};



using MalNode = std::shared_ptr<MalType>;

struct List : MalType {
    List() : MalType{NodeType::List}, children_{} {}
    ~List() override  {}

    void Add(MalNode node);
    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    std::vector<MalNode> children_;
};

struct Vector : MalType {
    Vector() : MalType{NodeType::Vector}, children_{} {}
    ~Vector() override { }

    void Add(MalNode node);
    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    std::vector<MalNode> children_;
};

struct HashMap : MalType {
    HashMap() : MalType{NodeType::HashMap}, kv_{} {}
    ~HashMap() override  {}

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    std::unordered_map<std::string, MalNode> kv_;
};

struct Int : MalType {
    explicit Int(int i) : MalType{NodeType::Int}, num_{i} {}
    ~Int() override { }

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    int num_;
};

struct Double : MalType {
    explicit Double(double d) : MalType{NodeType::Double}, num_{d} {}
    ~Double() override {}

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    double num_;
};

struct Keyword : MalType {
    explicit Keyword(std::string keyword) : MalType(NodeType::Keyword), keyword_{keyword.substr(1, keyword.size() - 1)} {}
    ~Keyword() override {}

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    std::string keyword_;
};

struct String : MalType {
    explicit String(std::string s) : MalType{NodeType::String}, s_{s} {}
    ~String() override {}

    std::string PrintStr(bool print_readably);
    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    std::string s_;
};

struct Boolean : MalType {
    explicit Boolean(bool b) : MalType{NodeType::Boolean}, b_{b} {}
    ~Boolean() override {}

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    bool b_;
};

struct Symbol : MalType {
    explicit Symbol(char symbol) : MalType{NodeType::Symbol}, symbol_{symbol} {}
    explicit Symbol(std::string symbol) : MalType{NodeType::Symbol}, symbol_{symbol} { }
    ~Symbol() override {}

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    std::string symbol_;
};

struct Quote : MalType {
    explicit Quote(MalNode child) : MalType{NodeType::Quote}, child_{child} {}
    ~Quote() override {}

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    MalNode child_;
};

struct Quasiquote : MalType {
    explicit Quasiquote(MalNode child) : MalType{NodeType::Quasiquote}, child_{child} {}
    ~Quasiquote() override {}

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    MalNode child_;
};

struct Unquote : MalType {
    explicit Unquote(MalNode child) : MalType{NodeType::Unquote}, child_{child} {}
    ~Unquote() override {}

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;

    MalNode child_;
};


struct Nil : MalType {
    Nil() : MalType{NodeType::Nil} {}
    ~Nil() override {}

    std::string Print(bool print_readably) override;
    bool operator==(MalType& other) override;
};

using MalFunc = std::function<MalNode(std::vector<MalNode>&)>;
struct Function : MalType {
    Function(MalFunc func) : MalType{MalType::NodeType::Function}, func_{func} {}
    ~Function() override {}

    std::string Print(bool print_readably) override;
    MalNode ApplyFn(std::vector<MalNode>& nodes);
    bool operator==( [[maybe_unused]] MalType& other) override;

    std::function<MalNode(std::vector<MalNode>&)> func_;
};



#endif // MAL_TYPES_H