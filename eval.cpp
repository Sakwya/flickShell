#include "eval.h"

#include "builtin_cd.h"
#include "builtin_help.h"
#include "builtin_history.h"
#include "builtin_exit.h"
#include "builtin_type.h"
#include <map>
#include <string>
#include <vector>


std::map<std::string, std::string> alias_map;
std::map<std::string, std::string> help_map;
std::string prompt;
int pipe_fd[2];

void init_shell() {
    alias_map.insert({ {"ll", "ls -l"} });
}

void init_help() {
    help_map.insert({ {"cd", "cd: cd [-L|[-P [-e]] [-@]] [dir]\n    Change the shell working directory.\n    Change the current directory to DIR.  The default DIR is the value of the\n    HOME shell variable."} });
}


// deal with builtin command
// returns: 0-nothing_done, 1-success, -1-failure
int process_builtin_command(const std::string& line) {
    std::vector<std::string> args = string_split(line, WHITE_SPACE);
    // 1 - cd
    if (args[0] == "cd") {
        change_directory(args);
        return 1; // successfully processed
    }
    // 2 - help
    if (args[0] == "help") {
        display_help(args);
        return 1;

    }
    try {
        if (args[0] == "cd") {
            return cd_command(args);
        } else if (args[0] == "help") {
            return help_command(args);
        } else if (args[0] == "exit") {
            return exit_command(args);
        } else if (args[0] == "history") {
            return history_command(args);
        }

        show_history();
        return 1;
    }

    // 5 - type
    if (args[0] == "tyoe") {
        show_type(args);
        return 1;
    }
    // 6 - exit
    if (args[0] == "exit") {
        exit_shell(args);
        return 1;

    }
    return 0;
}

void reader_loop() {
    prompt = get_command_prompt();
    std::string line;
    int wait_status;
    while (true) {
        line = trim(readline(prompt.c_str()));
        if (line.empty()) continue;
        add_history(line.c_str());
        if (process_builtin_command(line) > 0) continue;
        if (fork_wrap() == 0) {
            cmd* cmd_ = parse(line);
            run_cmd(cmd_);
            exit(0);
        }
        wait(&wait_status);
        check_wait_status(wait_status);
    }
}

int main() {
    init_alias();
    init_help();
    reader_loop();
    return 0;
}
