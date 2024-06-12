#include "builtin_help.h"
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <eval.h>

std::map<std::string, std::string> help_map;

void init_help() { 
    help_map.insert({ {"cd","cd: cd [-L|[-P [-e]] [-@]] [dir]\n    Change the shell working directory.\n    Change the current directory to DIR.  The default DIR is the value of the\n    HOME shell variable."} }); 
}

void display_help(const std::vector<std::string>& args) {
    std::string key;
    switch (args.size()) {
    case 1:
        //   cout << "" << endl;
        break;
    case 2:
        if (args[1].size() > 256) {
            panic("input out of limit");
        }
        try {
            key = args[1];
            std::string value = help_map.at(key);
            std::cout << value << std::endl;
        } catch (const std::out_of_range& e) {
            char char_buf[256];
            std::snprintf(char_buf, 256, "no help topics match `%s'.  Try `help help' or `man -k %s' or `info %s'.", key.c_str(), key.c_str(), key.c_str());
            panic(char_buf);
        }
        break;
    default:
        panic("too many arguments");
    }
}
