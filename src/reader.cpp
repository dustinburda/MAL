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

        if (match_str[0] == ';')
            continue;

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
    while (Peek().has_value() && Peek().value() != "}" && Peek().value() != "") {
        auto key = Next().value();
        auto val = ReadForm();

        kv[key] = val;
    }

    auto terminator = Next().value(); // '}'
    if (terminator != "}")
        throw std::logic_error("unbalanced");

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
        case '*':
        case '/': {
            node = ReadSymbol();
            break;
        }
        case '-': {
            if (token[0] == '-' && std::isdigit(token[1])) {
                node = ReadNum();
                break;
            } else {
                node = ReadSymbol();
                break;
            }
        }
        case ':': {
            Next();
            node = std::make_shared<Keyword>(token);
            break;
        }
        case '"' : {
            node = ReadString();
            break;
        }
        case '`': {
            node = ReadQuote<Quasiquote>();
            break;
        }
        case '\'': {
            node = ReadQuote<Quote>();
            break;
        }
        case '~': {
            node = ReadQuote<Unquote>();
            break;
        }
        default : {
            if (std::isdigit(token[0])) {
                node = ReadNum();
                break;
            } else if (token == "nil") {
                Next();
                node = std::make_shared<Nil>();
                break;
            } else if (token[0] == '\"' && token.back() == '\"') {
                node = ReadString();
                break;
            } else if (token == "true" || token == "false") {
                Next();
                node = (token == "true") ? std::make_shared<Boolean>(true) : std::make_shared<Boolean>(false);
                break;
            } else if (std::isgraph(token[0])) {
                Next();
                node = std::make_shared<Symbol>(token);
                break;
            } else {
                node = std::make_shared<Nil>();
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
    auto token = Next().value();
    if( token[0] != token.back() || token.size() == 1)
        throw std::logic_error("unbalanced");

    std::string value;

    std::string literal_value = token.substr(1, token.size() - 2);
    for(std::size_t index = 0; index < literal_value.size(); index++)
    {
//        if (literal_value[index] == '\\' && index < literal_value.size() - 1) {
//            switch (literal_value[index + 1]) {
//                case '\n': {
//                    value += '\n';
//                    index++;
//                    continue;
//                }
//                case '\\': {
//                    value += '\\';
//                    index++;
//                    continue;
//                }
//                case '\"': {
//                    value += '\"';
//                    index++;
//                    continue;
//                }
//            }
//        }

        value += literal_value[index];
    }


    return std::make_shared<String>(value);
}