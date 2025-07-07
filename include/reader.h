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

    template<typename QuoteType>
    MalNode ReadQuote();

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
 * @brief Creates a quote/quasiquote node
 *
 * @return AST Quote/Quasiquote node
 * */
template<typename QuoteType>
MalNode Reader::ReadQuote() {
    [[maybe_unused ]] auto quote = Next().value();
    auto node = ReadForm();
    return std::make_shared<QuoteType>(node);
}

/*
 * @brief Reads a sequence of child nodes into a list or vector structure
 *
 * @return AST List/Vector node
 * */
template<typename ListType>
MalNode Reader::ReadSequence() {
    MalNode node = std::make_shared<ListType>();

    Next(); // '(', '['

    std::string termination_string = (std::is_same<ListType, List>::value) ? ")" : "]";

    while (Peek().has_value() && Peek().value() != termination_string && Peek().value() != "") {
        static_cast<ListType*>(node.get())->children_.push_back(ReadForm());
    }

    auto actual_terminator = Next().value(); // ')', ']'
    if (actual_terminator !=  termination_string)
        throw std::logic_error("unbalanced");

    return node;
}

#endif // MAL_READER_H