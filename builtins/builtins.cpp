#include <builtins/builtins.h>
#include <panic.h>
#include <unordered_set>
#include <unordered_map>

#include <builtins/alias.h>
#include <builtins/cd.h>
#include <builtins/help.h>
#include <builtins/history.h>
#include <builtins/exit.h>
#include <builtins/type.h>
#include <bashline.h>

std::unordered_set<std::string> builtins_set{
    "alias",
    "cd",
    "help",
    "exit",
    "type",
    // "pwd",
    // "echo",
    // "env",
    // "export",
    // "unset",
    "unalias",
    "history",
};
std::unordered_map<std::string, void (*)(const std::vector<std::string>&)> builtins_map{
    {"alias",&alias},
    {"cd",&change_directory},
    {"help",&display_help},
    {"exit",&exit_shell},
    {"unalias",&alias},
    {"history",&show_history},
    {"type",&show_type}
};
bool is_builtin(const std::string name) {
    auto it = builtins_set.find(name);
    return (it != builtins_set.end());
}

void run_builtin(const std::string name, const std::vector<std::string>& args) {
    auto it = builtins_map.find(name);
    if (it != builtins_map.end()) {
        it->second(args);
        return;
    }
    std::string message = name;
    message.append(" is not a builtin function.");
    panic(message, true, 1);
    return;
}