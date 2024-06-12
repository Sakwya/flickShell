#include "eval.h"
#include "builtins/cd.h"
#include "builtins/help.h"
#include "builtins/exit.h"
#include "builtins/history.h"

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

int process_builtin_command(std::string line) {
    std::vector<std::string> args = string_split(line, " ");
    if (args.empty()) {
        return 0;
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
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return -1;
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
