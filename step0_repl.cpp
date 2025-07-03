#include <iostream>


std::string READ(std::string line) {
    return line;
}

std::string EVAL(std::string ast) {
    return ast;
}

std::string PRINT(std::string expression) {
    return expression;
}

std::string rep(std::string line) {
    return READ(EVAL(PRINT(line)));
}

int main() {
    std::string line;

	while (true) {
        std::cout << "user> ";

        if (std::cin.eof())
            std::exit(1);

        std::string line;
        std::getline(std::cin, line);

        std::cout << rep(line) << std::endl;
    }
}
