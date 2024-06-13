#include <builtins/help.h>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <panic.h>
#include <map>

std::map<std::string, std::string> help_map{
        {"cd", "cd: cd [-L|[-P [-e]] [-@]] [dir]\n    更改shell工作目录。\n    将当前目录更改为DIR。默认的DIR是HOME shell变量的值。"},
        {"exit", "exit: exit [n]\n    退出shell。\n    退出当前的shell会话。如果提供了n，将返回退出状态n。"},
        {"echo", "echo: echo [arg ...]\n    显示一段文本。\n    将参数作为输出显示在终端上。"},
        {"pwd", "pwd: pwd\n    打印当前工作目录。\n    显示当前的工作目录路径。"},
        {"ls", "ls: ls [选项] [文件...]\n    列出目录内容。\n    显示指定目录中的文件和目录。如果没有指定目录，则显示当前目录的内容。"},
        {"mkdir", "mkdir: mkdir [选项] name...\n    创建目录。\n    创建指定名称的目录。"},
        {"rmdir", "rmdir: rmdir [选项] name...\n    删除目录。\n    删除指定名称的空目录。"},
        {"rm", "rm: rm [选项] 文件...\n    删除文件或目录。\n    删除指定的文件或目录。如果目录非空，则需要使用选项来递归删除。"},
        {"cp", "cp: cp [选项] 源... 目标\n    复制文件或目录。\n    将源文件或目录复制到目标位置。"},
        {"mv", "mv: mv [选项] 源... 目标\n    移动（重命名）文件。\n    将源文件或目录移动到目标位置。"},
        {"cat", "cat: cat [文件...]\n    连接文件并打印到标准输出。\n    显示文件的内容，如果有多个文件，将它们依次连接并显示。"},
        {"touch", "touch: touch [选项] 文件...\n    更改文件的访问和修改时间。\n    如果文件不存在，将创建一个空文件。"},
        {"man", "man: man [命令]\n    显示命令的手册页。\n    显示指定命令的使用手册。"},
        {"chmod", "chmod: chmod [选项] mode 文件...\n    更改文件模式比特。\n    更改指定文件的访问权限。"},
        {"chown", "chown: chown [选项] owner[:group] 文件...\n    更改文件所有者和组。\n    更改指定文件的所有者和/或组。"},
        {"find", "find: find [路径] [选项]\n    在目录层次结构中搜索文件。\n    根据指定条件搜索目录中的文件。"},
        {"grep", "grep: grep [选项] 模式 文件...\n    搜索文本。\n    在文件中搜索符合模式的行。"},
        {"kill", "kill: kill [选项] [信号] PID...\n    终止进程。\n    向指定的进程发送信号（默认是SIGTERM）。"},
        {"ps", "ps: ps [选项]\n    报告当前进程状态。\n    显示当前正在运行的进程列表和状态信息。"},
        {"history", "history: history [选项] [n]\n    显示命令历史。\n    列出最近执行的命令。如果提供了数字n，则显示最近的n条命令。"},
        {"type", "type: type [-a] name [name...]\n    显示命令类型。\n    显示指定命令的类型（别名、函数、内建命令、可执行文件等）。"},
        {"alias", "alias: alias [name[=value] ...]\n    创建命令别名。\n    定义或显示别名。无参数时，显示所有别名。"}
    };

void display_help(const std::vector<std::string>& args) {
    std::string key;
    switch (args.size()) {
    case 1:
        // 如果没有提供具体命令，显示所有可用命令的概要
        std::cout << "可用命令:\n";
        for (const auto& pair : help_map) {
            std::cout << "  " << pair.first << "    - " << pair.second.substr(0, pair.second.find('\n')) << "\n";
        }
        break;
    case 2:
        if (args[1].size() > 256) {
            panic("输入超出限制");
        }
        try {
            key = args[1];
            std::string value = help_map.at(key);
            std::cout << value << std::endl;
        } catch (const std::out_of_range& e) {
            char char_buf[256];
            std::snprintf(char_buf, 256, "没有与 `%s` 匹配的帮助主题。尝试 `help help` 或 `man -k %s` 或 `info %s`。", key.c_str(), key.c_str(), key.c_str());
            panic(char_buf);
        }
        break;
    default:
        panic("参数过多");
    }
}
