#include <vector>
#include <string>
#include <map>

extern std::map<std::string, std::string> help_map;

void init_help();
void display_help(const std::vector<std::string>& args);
