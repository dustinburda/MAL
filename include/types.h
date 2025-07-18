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
    virtual std::string Print() = 0;
    virtual bool operator==(MalType& other) = 0;

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

    bool operator==(MalType& other) override{
        if (type_ != other.type_)
            return false;

        auto other_list = static_cast<List*>(&other);
        if (other_list->children_.size() != children_.size())
            return false;

        for (std::size_t index = 0; index < children_.size(); index++) {
            if (! (*children_[index] == *other_list->children_[index]))
                return false;
        }

        return true;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        auto other_list = static_cast<Vector*>(&other);
        if (other_list->children_.size() != children_.size())
            return false;

        for (std::size_t index = 0; index < children_.size(); index++) {
            if (! (*children_[index] == *other_list->children_[index]))
                return false;
        }

        return true;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        auto other_map = static_cast<HashMap*>(&other);
        if (other_map->kv_.size() != other_map->kv_.size())
            return false;

        for (auto& [k, v] : kv_) {
            if (other_map->kv_.count(k) == 0)
                return false;
            if(! (other_map->kv_[k] == v))
                return false;
        }

        return true;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        return num_ == static_cast<Int*>(&other)->num_;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        return num_ == static_cast<Double*>(&other)->num_;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        return keyword_ == static_cast<Keyword*>(&other)->keyword_;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        return s_ == static_cast<String*>(&other)->s_;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        return b_ == static_cast<Boolean*>(&other)->b_;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        return symbol_ == static_cast<Symbol*>(&other)->symbol_;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        return child_ == static_cast<Quote*>(&other)->child_;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        return child_ == static_cast<Quasiquote*>(&other)->child_;
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

    bool operator==(MalType& other) override {
        if (other.type_ != type_)
            return false;

        return child_ == static_cast<Unquote*>(&other)->child_;
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

    bool operator==(MalType& other) override {
        return other.type_ == type_;
    }
};

using MalFunc = std::function<MalNode(std::vector<MalNode>&)>;
struct Function : MalType {
    Function(MalFunc func) : MalType{MalType::NodeType::Function}, func_{func} {}
    ~Function() override {}

    std::string Print() override {
        std::stringstream ss;

        ss << "function";

        return ss.str();
    }

    MalNode ApplyFn(std::vector<MalNode>& nodes) {
        return func_(nodes);
    }

    bool operator==( [[maybe_unused]] MalType& other) override {
        return true;
    }

    std::function<MalNode(std::vector<MalNode>&)> func_;
};



#endif // MAL_TYPES_H