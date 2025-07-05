#include "../include/printer.h"

std::string PrintAst( [[ maybe_unused ]] MalNode node) {
    return node->Print();
}