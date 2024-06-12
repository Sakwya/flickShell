#include "builtin_cd.h"
#include "eval.h"
#include <unistd.h>
#include <string>

extern std::string home_dir;
extern std::string prompt;

void change_directory(const std::vector<std::string>& args) {
    std::string route;
    int chdir_ret;
    switch (args.size()) {
    case 1:
        chdir(home_dir.c_str()); // single cd means cd \~
        prompt = get_command_prompt();
        break;
    case 2:
        route = args[1];
        if (route.find("/~") == 0) {
            route = home_dir + route.substr(1);
        }
        // change directory
        chdir_ret = chdir(route.c_str());
        if (chdir_ret < 0) {
            panic("chdir failed");
        }
        prompt = get_command_prompt();
        break;
    default:
        panic("too many arguments");
    }
}
