//
// Created by Dustin Burda on 12/9/23.
//

#ifndef MAL_MASTER_READER_H
#define MAL_MASTER_READER_H

#include <string>
#include <vector>
#include <regex>
#include <optional>
#include "Types.h"

// TODO: switch to pointers

static const std::regex lisp_reg {"[\\s,]*(~@|[\\[\\]{}()'`~^@]|\"(?:\\\\.|[^\\\\\"])*\"?|;.*|[^\\s\\[\\]{}('\"`,;)]*)"};
using Token = std::string;

class Reader {
public:
    Reader() = default;
    explicit Reader(std::vector<Token> tokens) : pos_{0}, tokens_{std::move(tokens)} {}

    std::optional<std::string> Next() {
        if(pos_ == tokens_.size() -1 )
            return std::nullopt;

        pos_++;

        return tokens_[pos_ - 1];
    }

    std::optional<std::string> Peek() const {
        if(pos_ >= tokens_.size())
            return std::nullopt;

        return tokens_[pos_];
    }

private:
    size_t pos_;
    std::vector<std::string> tokens_;
};


static mal_t_ptr read_list(Reader& reader) {
    mal_t_ptr list;

    while(reader.Peek() != std::nullopt) {

    }

    return list;
}

static mal_t_ptr read_atom(Reader& reader) {
    mal_t_ptr t;
    return t;
}

static mal_t_ptr read_form(Reader& reader) {
    auto curr_token = reader.Peek().value();

    mal_t_ptr type;
    switch(curr_token[0]) {
        case '(':
            type = read_list(reader);
            break;
        default:
            type = read_atom(reader);

    }

    return type;
}

static void tokenize(std::string& src, std::vector<Token>& tokens) {
    auto begin_it = std::sregex_iterator(src.begin(), src.end(), lisp_reg);
    auto end_it = std::sregex_iterator();

    for(auto it = begin_it; it != end_it; ++it) {
        std::smatch matches = *it;

        if(matches.size() <= 1 || matches[1].str().empty() )
            continue;

        tokens.push_back( matches[1].str() );

    }
}

[[maybe_unused]] static void read_str(std::string src) {

    std::vector<Token> tokens;
    tokenize(src, tokens);

    // Reader consumes tokens
    Reader reader { tokens };

    read_form(reader);
}

#endif //MAL_MASTER_READER_H
