#include "../include/reader.h"

std::optional<std::string> Reader::Next() {
    if (index_ >= tokens_.size())
        return std::nullopt;

    auto token = tokens_[index_];
    index_++;

    return token;
}

std::optional<std::string> Reader::Peek() {
    if (index_ >= tokens_.size())
        return std::nullopt;

    return tokens_[index_];
}

void Reader::Tokenize() {
    std::regex lisp_regex {lisp_regex_str};

    auto tokens_begin = std::sregex_iterator (src_.begin(), src_.end(), lisp_regex);
    auto tokens_end = std::sregex_iterator ();

    for (auto i = tokens_begin; i != tokens_end; i++) {
        std::smatch match = *i;
        std::string match_str = match[1].str();
        tokens_.push_back(match_str);
    }
}

MalNode Reader::ReadForm() {
    if (Peek() == std::nullopt)
        return nullptr;

    auto token = Peek().value();

    MalNode node = nullptr;
    switch (token[0]) {
        case '(' : {
            node = ReadSequence<List>();
            break;
        }
        case '[' : {
            node = ReadSequence<Vector>();
            break;
        }
        case '{': {
            node = ReadHashMap();
            break;
        }
        default: {
            node = ReadAtom();
            break;
        }
    }

    return node;
}

MalNode Reader::ReadHashMap() {
    Next(); // '{'

    std::unordered_map<std::string, MalNode> kv;
    while (Peek().has_value() && Peek().value() != "}") {
        auto key = Next().value();
        auto val = ReadForm();

        kv[key] = val;
    }

    Next(); // '}'
    auto node = std::make_shared<HashMap>();
    node->kv_ = std::move(kv);

    return node;
}

MalNode Reader::ReadAtom() {
    if (Peek() == std::nullopt)
        return nullptr;

    auto token = Peek().value();

    MalNode node = nullptr;

    switch (token[0]) {
        case '+':
        case '-':
        case '*':
        case '/': {
            node = ReadSymbol();
            break;
        }
        case ':': {
            node = std::make_shared<Keyword>(token);
            break;
        }
        case '"' : {
            node = ReadString();
            break;
        }
        default : {
            if (isdigit(token[0])) {
                node = ReadNum();
                break;
            } else if (token == "nil") {
                Next();
                node = std::make_shared<Nil>();
                break;
            } else if (token[0] == '\"' && token.back() == '\"') {
                // TODO This isn't right
                node = ReadString();
                break;
            } else if (token == "true" || token == "false") {
                Next();
                node = (token == "true") ? std::make_shared<Boolean>(true) : std::make_shared<Boolean>(false);
                break;
            } else if (std::isalpha(token[0])) {
                Next();
                node = std::make_shared<Symbol>(token);
                break;
            }
        }
    }

    return node;
}

MalNode Reader::ReadSymbol() {
    auto token = Next().value();
    return std::make_shared<Symbol>(token);
}

MalNode Reader::ReadNum() {
    auto token = Next().value();

    MalNode node = nullptr;
    if (token.find('.') != std::string::npos)
        node = std::make_shared<Double>(std::stod(token));
    else
        node = std::make_shared<Int>(std::stoi(token));

    return node;
}

MalNode Reader::ReadString() {
    return nullptr;
}