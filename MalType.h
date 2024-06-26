#ifndef MALTYPE_H_
#define MALTYPE_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <stdexcept>

#define EQUAL_EQUAL 1

struct MalType;
using mal_t_ptr = std::shared_ptr<MalType>;

enum class Mal_T {
    List,
    Vec,
    Map,
    Number,
    Symbol,
    String,
    Bool,
    Nil,
    Function,
    Keyword,
    Quote
};

// // TODO: implement equality in all cases


struct MalType {
    MalType(Mal_T mal_t) : type { mal_t }  {};
    virtual ~MalType() {};

#if EQUAL_EQUAL
    virtual bool operator==(const MalType& other) = 0;
#endif //EQUAL_EQUAL

    Mal_T get_type() { return type; }
    Mal_T type;
};

struct MalList : MalType {
    MalList() : MalType { Mal_T::List } {}

    void add(const mal_t_ptr& mal_t) {
        mal_list_.push_back(mal_t);
    }

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {

        // cast other into MalList&
        const auto& other_list = dynamic_cast<const MalList&>(other);
        if(mal_list_.size() != other_list.mal_list_.size()) {
            return false;
        }

        size_t length = mal_list_.size();

        for(size_t i = 0; i < length; i++) {
            if( !(*mal_list_[i] == *other_list.mal_list_[i]) ) {
                return false;
            }
        }

        return true;
    }
#endif // EQUAL_EQUAL

    std::vector<mal_t_ptr> mal_list_;
};


struct MalVec : MalType {
    MalVec() : MalType { Mal_T::Vec } {}

    void add(mal_t_ptr& mal_t) {
        mal_vec_.push_back(mal_t);
    }

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {

        // cast other into MalVec&
        const auto& other_vec = dynamic_cast<const MalVec&>(other);
        if(mal_vec_.size() != other_vec.mal_vec_.size()) {
            return false;
        }

        size_t length = mal_vec_.size();

        for(size_t i = 0; i < length; i++) {
            if( !(*mal_vec_[i] == *other_vec.mal_vec_[i]) ) {
                return false;
            }
        }

        return true;
    }
#endif //EQUAL_EQUAL
    
    std::vector<mal_t_ptr> mal_vec_;
};
//======================================================
struct MalMap : MalType {
    MalMap() : MalType { Mal_T::Map} {}

    void add(mal_t_ptr k, mal_t_ptr v) {
        malmap_[k] = v;
    }

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {

        // cast other into MalVec&
        const auto& other_map = dynamic_cast<const MalMap&>(other);
        if(malmap_.size() != other_map.malmap_.size()) {
            return false;
        }

        size_t length = malmap_.size();

        for(const auto& [k, v] : malmap_) {
            if( other_map.malmap_.count(k) == 0 || !(*v == *(other_map.malmap_.at(k))) ) {
                return false;
            }
        }

        return true;
    }
#endif //EQUAL_EQUAL

    std::unordered_map<mal_t_ptr, mal_t_ptr> malmap_;
};
//=====================================================


struct MalNumber : MalType {
    explicit MalNumber(int number) : MalType {Mal_T::Number},  number_{ number } {}

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {
        // cast other into MalNumber&
        const auto& other_num = dynamic_cast<const MalNumber&>(other);

        return number_ == other_num.number_;
    }
#endif //EQUAL_EQUAL
    int number_;
};

struct MalSymbol : MalType {
    explicit MalSymbol(std::string symbol) : MalType {Mal_T::Symbol}, symbol_ { std::move(symbol) } {}

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {
        // cast other into MalNumber&
        const auto& other_symbol = dynamic_cast<const MalSymbol&>(other);

        return symbol_ == other_symbol.symbol_;
    }
#endif // EQUAL_EQUAL

    std::string symbol_;
};


struct MalString : MalType {
    explicit MalString(std::string s) : MalType {Mal_T::String}, string_ { std::move(s) } {}

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {
        // cast other into MalNumber&
        const auto& other_string = dynamic_cast<const MalString&>(other);

        return string_ == other_string.string_;
    }
#endif // EQUAL_EQUAL
    std::string string_;
};

struct MalBool : MalType {
    explicit MalBool(std::string token) : MalType {Mal_T::Bool}, bool_ {token == "true"} {}
    explicit MalBool(bool bool_v) : MalType {Mal_T::Bool}, bool_ {bool_v} {}

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {
        // cast other into MalNumber&
        const auto& other_bool = dynamic_cast<const MalBool&>(other);

        return bool_ == other_bool.bool_;
    }
#endif // EQUAL_EQUAL

    bool bool_;
};

struct MalNil : MalType {
    explicit MalNil(std::string nil) : MalType { Mal_T::Nil}, nil_{nil} {}

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {
        return true;
    }
#endif // EQUAL_EQUAL

    std::string nil_;
};

struct MalFunction : MalType {
    explicit MalFunction(std::function<mal_t_ptr(std::vector<mal_t_ptr>)>f) : MalType {Mal_T::Function}, fn { std::move(f) } {}

    mal_t_ptr apply_fn(const std::vector<mal_t_ptr>& args) const {
        return fn(args);
    }

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {
        return true; // TODO: not sure what this should be
    }
#endif // EQUAL_EQUAL

    std::function<mal_t_ptr(std::vector<mal_t_ptr>)> fn;
};

struct MalKeyword : MalType {
    explicit MalKeyword(std::string keyword) : MalType { Mal_T::Keyword }, keyword_ { keyword } {}

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {
        // cast other into MalNumber&
        const auto& other_keyword = dynamic_cast<const MalKeyword&>(other);

        return keyword_ == other_keyword.keyword_;
    }
#endif //EQUAL_EQUAL

    std::string keyword_;
};

struct MalQuote : MalType {
    explicit MalQuote(std::string quote_symbol) : MalType { Mal_T::Quote}, quote_symbol_ { quote_symbol } {}

#if EQUAL_EQUAL
    bool operator==(const MalType& other) override {
        // cast other into MalNumber&
        const auto& other_quote = dynamic_cast<const MalQuote&>(other);

        return quote_symbol_ == other_quote.quote_symbol_;
    }
#endif //EQUAL_EQUAl

    std::string quote_symbol_;
};

#endif // MALTYPE_H_