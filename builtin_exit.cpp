#include "builtin_exit.h"
#include <iostream>
#include <cstdlib>
#include <eval.h>

void exit_shell(const std::vector<std::string>& args) {
    switch (args.size()) {
    case 1:
        std::cout << "Bye from flickShell." << std::endl;
        exit(0);
        break;
    case 2:
        try {
            int num = std::stoi(args[1]);
            exit(num);
        } catch (const std::invalid_argument& e) {
            panic("Invalid argument");
        }
        break;
    default:
        panic("too many arguments");
    }
}
