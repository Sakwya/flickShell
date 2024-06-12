#include "type.h"
#include "eval.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <cerrno>
#include <map>

// 获取文件的绝对路径
std::string get_absolute_path(const std::string& path) {
    char abs_path[PATH_MAX];
    if (realpath(path.c_str(), abs_path) != nullptr) {
        return std::string(abs_path);
    } else {
        return path; // 如果无法获取绝对路径，则返回原始路径
    }
}

// 假设的内置命令列表
const std::map<std::string, bool> builtin_commands = {
    {"cd", true},
    {"exit", true},
    {"pwd", true},
    {"echo", true},
    {"env", true},
    {"export", true},
    {"unset", true},
    {"alias", true},
    {"unalias", true},
    {"history", true},
    {"help", true}
};

// 假设的别名列表
const std::map<std::string, std::string> aliases = {
    {"ll", "ls -l"},
    {"la", "ls -a"},
    {"lt", "ls -lt"},
    {"lS", "ls -S"},
    {"grep", "grep --color=auto"},
    {"ls", "grep --color=auto"},    
};


enum CommandType {
    NOT_FOUND,
    BUILTIN,
    ALIAS
};

CommandType is_builtin_or_alias(const std::string& name) {
    // 检查是否是内置命令
    if (builtin_commands.find(name) != builtin_commands.end()) {
        return BUILTIN;
    }
    // 检查是否是别名
    if (aliases.find(name) != aliases.end()) {
        return ALIAS;
    }
    // 如果都不是，返回false
    return NOT_FOUND;
}

void show_type(const std::vector<std::string>& args) {
    // 检查参数数量
    if (args.size() != 2) {
        panic("invalid number of arguments");
    }
    // 获取要显示类型的命令或文件名
    const std::string& name = args[1];
    struct stat file_stat;
    std::string command_path;

    // 检查是否是内置命令或别名
    CommandType commandType = is_builtin_or_alias(name);
    switch (commandType) {
        case BUILTIN:
            std::cout << "builtin command: " << name << std::endl;
            return;
        case ALIAS:
            std::cout << "alias command: " << name << " -> " << aliases.at(name) << std::endl;
            return;
        case NOT_FOUND:
            // 尝试找到命令的路径
            command_path = "../flichShell/builtins/" + name;
            if (stat(command_path.c_str(), &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                std::cout << "command (binary) found at: " << get_absolute_path(command_path) << std::endl;
                return;
            }
            // 如果不是命令，检查是否为文件或目录
            if (stat(name.c_str(), &file_stat) == 0) {
                if (S_ISDIR(file_stat.st_mode)) {
                    std::cout << "directory found at: " << get_absolute_path(name) << std::endl;
                } else if (S_ISREG(file_stat.st_mode)) {
                    std::cout << "regular file found at: " << get_absolute_path(name) << std::endl;
                    // 打印文件内容（省略，使用之前定义的print_file_contents函数）
                } else {
                    std::cout << "other type of file found at: " << get_absolute_path(name) << std::endl;
                    // 处理其他文件类型
                }
            } else {
                // stat调用失败，可能是因为文件不存在或无法访问
                std::cout << "entity does not exist or cannot be accessed: " << name << std::endl;
            }
            return;
    }
}
