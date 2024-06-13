#include <VECTOR.h>
#include <STRING.h>
// extern std::unordered_set<std::string> builtins_set;
extern bool is_builtin(const std::string);
extern void run_builtin(const std::string, const std::vector<std::string>&);