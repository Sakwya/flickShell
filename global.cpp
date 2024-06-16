#include <global.h>
std::string home_dir;
std::string prompt;
std::vector<std::string> flickshrc;

std::unordered_map<std::string, std::string> alias_map;
std::unordered_map<std::string, std::string> command_map;
std::unordered_set<std::string> builtins_set{
    "alias", "cd", "help", "exit", "type", "export", "unalias", "history",
};