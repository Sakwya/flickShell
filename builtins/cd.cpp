#include <builtins/cd.h>
#include <unistd.h>
#include <global.h>
#include <panic.h>
#include <bashline.h>
void change_directory(const std::vector<std::string>& args) {
    std::string route;
    int chdir_ret;
    switch (args.size()) {
    case 1:
        chdir(home_dir.c_str()); // single cd means cd \~
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
        break;
    default:
        panic("too many arguments");
    }
    set_prompt();
    return;
}
