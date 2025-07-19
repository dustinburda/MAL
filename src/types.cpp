#include "../include/types.h"

void List::Add(MalNode node) {
    children_.push_back(node);
}

std::string List::Print() {
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

bool List::operator==(MalType& other) {
    if (other.type_ != NodeType::List && other.type_ != NodeType::Vector)
        return false;

    auto other_list_children = (other.type_ != NodeType::Vector) ? static_cast<Vector*>(&other)->children_ : static_cast<List*>(&other)->children_;
    if (other_list_children .size() != children_.size())
        return false;

    for (std::size_t index = 0; index < children_.size(); index++) {
        if (! (*children_[index] == *other_list_children [index]))
            return false;
    }

    return true;
}

bool Vector::operator==(MalType& other) {
    if (other.type_ != NodeType::List && other.type_ != NodeType::Vector)
        return false;

    auto other_list_children = (other.type_ != NodeType::Vector) ? static_cast<Vector*>(&other)->children_ : static_cast<List*>(&other)->children_;
    if (other_list_children.size() != children_.size())
        return false;

    for (std::size_t index = 0; index < children_.size(); index++) {
        if (! (*children_[index] == *other_list_children[index]))
            return false;
    }

    return true;
}

void Vector::Add(MalNode node) {
    children_.push_back(node);
}

std::string Vector::Print() {
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

std::string HashMap::Print() {
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

bool HashMap::operator==(MalType& other) {
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


std::string Int::Print() {
    std::stringstream ss;

    ss << num_;

    return ss.str();
}

bool Int::operator==(MalType& other) {
    if (other.type_ != type_)
        return false;

    return num_ == static_cast<Int*>(&other)->num_;
}

std::string Double::Print() {
    std::stringstream ss;

    ss << num_;

    return ss.str();
}

bool Double::operator==(MalType& other) {
    if (other.type_ != type_)
        return false;

    return num_ == static_cast<Double*>(&other)->num_;
}

std::string Keyword::Print() {
    std::stringstream ss;

    ss << ":";
    ss << keyword_;

    return ss.str();
}

bool Keyword::operator==(MalType& other) {
    if (other.type_ != type_)
        return false;

    return keyword_ == static_cast<Keyword*>(&other)->keyword_;
}

std::string String::PrintStr(bool print_readably) {
    std::stringstream ss;

    for (std::size_t index = 0; index < s_.size(); index++) {
        if (print_readably && s_[index] == '\"')
            ss << '\\' << '\"';
        else if (print_readably && s_[index] == '\n')
            ss << '\\' << 'n';
        else if (print_readably && s_[index] == '\\')
            ss << '\\' << '\\';
        else
            ss << s_[index];
    }

    return ss.str();
}

std::string String::Print() {
    std::stringstream ss;
    ss << "\"";
    ss << PrintStr(true);
    ss << "\"";

    return ss.str();
}

bool String::operator==(MalType& other) {
    if (other.type_ != type_)
    return false;

    return s_ == static_cast<String*>(&other)->s_;
}

std::string Boolean::Print() {
    std::stringstream ss;

    ss << ((b_) ? "true" : "false");

    return ss.str();
}

bool Boolean::operator==(MalType& other) {
    if (other.type_ != type_)
    return false;

    return b_ == static_cast<Boolean*>(&other)->b_;
}

std::string Symbol::Print() {
    std::stringstream ss;

    ss << symbol_;

    return ss.str();
}

bool Symbol::operator==(MalType& other) {
    if (other.type_ != type_)
    return false;

    return symbol_ == static_cast<Symbol*>(&other)->symbol_;
}

std::string Quote::Print() {
    std::stringstream ss;

    ss << "(quote ";
    ss << child_->Print();
    ss << ")";

    return ss.str();
}

bool Quote::operator==(MalType& other) {
    if (other.type_ != type_)
    return false;

    return child_ == static_cast<Quote*>(&other)->child_;
}

std::string Quasiquote::Print() {
    std::stringstream ss;

    ss << "(quasiquote ";
    ss << child_->Print();
    ss << ")";

    return ss.str();
}

bool Quasiquote::operator==(MalType& other) {
    if (other.type_ != type_)
    return false;

    return child_ == static_cast<Quasiquote*>(&other)->child_;
}

std::string Unquote::Print() {
    std::stringstream ss;

    ss << "(unquote ";
    ss << child_->Print();
    ss << ")";

    return ss.str();
}

bool Unquote::operator==(MalType& other) {
    if (other.type_ != type_)
    return false;

    return child_ == static_cast<Unquote*>(&other)->child_;
}


