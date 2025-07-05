#ifndef MAL_READER_H
#define MAL_READER_H

#include <cstdint>
#include <optional>
#include <regex>
#include <string>
#include <vector>

#include "types.h"

using Token = std::string;

static std::string lisp_regex_str = "[\\s,]*(~@|[\\[\\]{}()'`~^@]|\"(?:\\\\.|[^\\\\\"])*\"?|;.*|[^\\s\\[\\]{}('\"`,;)]*)";

class Reader {
public:
    Reader(std::string src) : src_{src}, tokens_{}, index_{} {}

    void Tokenize();
    MalNode ReadForm();
    MalNode ReadAtom();
    MalNode ReadHashMap();
    MalNode ReadSymbol();
    MalNode ReadNum();
    MalNode ReadString();

    template<typename ListType>
    MalNode ReadSequence();

private:
    std::optional<std::string> Next();
    std::optional<std::string> Peek();

private:
    std::string src_;
    std::vector<Token> tokens_;
    std::size_t index_;
};


/*
 * @brief Reads a sequence of
 *
 * @return AST List/Vector node
 * */
template<typename ListType>
MalNode Reader::ReadSequence() {
    MalNode node = std::make_shared<ListType>();

    Next(); // '('

    while (Peek().has_value() && Peek().value() != ")") {
        static_cast<ListType*>(node.get())->children_.push_back(ReadForm());
    }

    return node;
}

#endif // MAL_READER_H