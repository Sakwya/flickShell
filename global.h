#ifndef GLOBAL_H
#define GLOBAL_H
#include <STRING.h>
#include <MAP.h>
#include <VECTOR.h>
extern std::string home_dir;
extern std::string prompt;
extern std::unordered_map<std::string, std::string> alias_map;
extern std::vector<std::string> flickshrc;
extern std::unordered_map<std::string, std::string> command_map;

#endif //GLOBAL_H