#include <iostream>
#include <string>
#include "Reader.h"
#include "Printer.h"


std::string READ(std::string str) {
    return str;
}

std::string EVAL(std::string ast, std::string env) {
    return ast;
}

std::string PRINT(std::string eval) {
    return eval;
}

std::string REP(std::string str) {
    return READ(EVAL(PRINT(str), ""));
}


int main() {
    // read_str("  (1 2, 3,,,,),,     ");

    std::string line;

    while(!std::cin.eof()) {
        std::cout << "user> ";

        std::getline(std::cin, line);

        std::cout << REP(line) << "\n";
    }
}