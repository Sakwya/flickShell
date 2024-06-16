#ifndef GLOBAL_H
#define GLOBAL_H
#include <STRING.h>
#include <VECTOR.h>
#include <unordered_set>
#include <unordered_map>

extern std::string home_dir;
extern std::string prompt;
extern std::unordered_map<std::string, std::string> alias_map;
extern std::vector<std::string> flickshrc;
extern std::unordered_map<std::string, std::string> command_map;
extern std::unordered_set<std::string> builtins_set;
#endif  // GLOBAL_H