#include <builtins/help.h>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <panic.h>
#include <map>

std::map<std::string, std::string> help_map{
    {"cd",
     "cd: cd [dir]\n"
     "    更改shell工作目录。\n"
     "    更改当前目录到指定的DIR。如果未指定DIR，则默认切换到HOME目录。\n"
     "    如果参数大于2， 则返回参数过多\n\n"
     "    例子:\n"
     "      cd          切换到HOME目录。\n"
     "      cd /path/to/dir  切换到指定的目录。\n"
     "      cd ~/docs   切换到HOME目录下的docs目录。\n"},
    {"exit",
     "exit: exit [n]\n"
     "    退出shell。\n"
     "    退出当前的shell会话，返回退出状态 n。\n\n"
     "    如果没有指定 n，退出状态默认为 0。\n\n"
     "    例子:\n"
     "      exit         退出shell，退出状态为 0。\n"
     "      exit 2       退出shell，退出状态为 2。\n"},
    {"echo",
     "echo: echo [arg ...]\n"
     "    显示一段文本。\n"
     "    将参数作为输出显示在终端上。\n\n"
     "    例子:\n"
     "      echo Hello World      显示字符串 'Hello World'\n"
     "      echo -n Hello          显示字符串 'Hello'，不换行\n"},
    {"pwd",
     "pwd: pwd\n"
     "    打印当前工作目录。\n"
     "    显示当前的工作目录路径。\n\n"
     "    例子:\n"
     "      pwd                   显示当前工作目录的路径\n"},
    {"ls",
     "ls: ls [选项] [文件...]\n"
     "    列出目录内容。\n"
     "    显示指定目录中的文件和目录。如果没有指定目录，则显示当前目录的内容。\n\n"
     "    例子:\n"
     "      ls                    列出当前目录下的所有文件和目录\n"
     "      ls -l /path/to/dir    列出指定目录下的详细信息\n"},
    {"mkdir",
     "mkdir: mkdir [选项] name...\n"
     "    创建目录。\n"
     "    创建指定名称的目录。\n\n"
     "    例子:\n"
     "      mkdir mydir          创建名为 'mydir' 的新目录\n"},
    {"rmdir",
     "rmdir: rmdir [选项] name...\n"
     "    删除目录。\n"
     "    删除指定名称的空目录。\n\n"
     "    例子:\n"
     "      rmdir mydir          删除名为 'mydir' 的空目录\n"},
    {"rm",
     "rm: rm [选项] 文件...\n"
     "    删除文件或目录。\n"
     "    删除指定的文件或目录。如果目录非空，则需要使用选项来递归删除。\n\n"
     "    例子:\n"
     "      rm myfile.txt        删除名为 'myfile.txt' 的文件\n"
     "      rm -r mydir          递归删除目录 'mydir' 及其内容\n"},
    {"cp",
     "cp: cp [选项] 源... 目标\n"
     "    复制文件或目录。\n"
     "    将源文件或目录复制到目标位置。\n\n"
     "    例子:\n"
     "      cp file1.txt file2.txt      将 'file1.txt' 复制为 'file2.txt'\n"
     "      cp -r dir1 /path/to/destination    复制 'dir1' 目录到指定目录\n"},
    {"mv",
     "mv: mv [选项] 源... 目标\n"
     "    移动（重命名）文件。\n"
     "    将源文件或目录移动到目标位置。\n\n"
     "    例子:\n"
     "      mv oldfile.txt newfile.txt      将 'oldfile.txt' 重命名为 'newfile.txt'\n"
     "      mv file1.txt /path/to/destination    移动 'file1.txt' 到指定目录\n"},
    {"cat",
     "cat: cat [文件...]\n"
     "    连接文件并打印到标准输出。\n"
     "    显示文件的内容，如果有多个文件，将它们依次连接并显示。\n\n"
     "    例子:\n"
     "      cat file1.txt file2.txt     显示 'file1.txt' 和 'file2.txt' 的内容\n"},
    {"touch",
     "touch: touch [选项] 文件...\n"
     "    更改文件的访问和修改时间。\n"
     "    如果文件不存在，将创建一个空文件。\n\n"
     "    例子:\n"
     "      touch newfile.txt          创建一个空的 'newfile.txt' 文件\n"},
    {"man",
     "man: man [命令]\n"
     "    显示命令的手册页。\n"
     "    显示指定命令的使用手册。\n\n"
     "    例子:\n"
     "      man ls                    显示 'ls' 命令的手册页\n"},
    {"chmod",
     "chmod: chmod [选项] mode 文件...\n"
     "    更改文件模式比特。\n"
     "    更改指定文件的访问权限。\n\n"
     "    例子:\n"
     "      chmod u+x script.sh       添加执行权限给 'script.sh' 文件的所有者\n"},
    {"chown",
     "chown: chown [选项] owner[:group] 文件...\n"
     "    更改文件所有者和组。\n"
     "    更改指定文件的所有者和/或组。\n\n"
     "    例子:\n"
     "      chown user1 file.txt      将 'file.txt' 的所有者更改为 'user1'\n"},
    {"find",
     "find: find [路径] [选项]\n"
     "    在目录层次结构中搜索文件。\n"
     "    根据指定条件搜索目录中的文件。\n\n"
     "    例子:\n"
     "      find . -name '*.txt'     在当前目录及其子目录中查找所有扩展名为 '.txt' 的文件\n"},
    {"grep",
     "grep: grep [选项] 模式 文件...\n"
     "    搜索文本。\n"
     "    在文件中搜索符合模式的行。\n\n"
     "    例子:\n"
     "      grep 'keyword' file.txt   在 'file.txt' 中查找含有 'keyword' 的行\n"},
    {"kill",
     "kill: kill [选项] [信号] PID...\n"
     "    终止进程。\n"
     "    向指定的进程发送信号（默认是SIGTERM）。\n\n"
     "    例子:\n"
     "      kill 1234               终止PID为 1234 的进程\n"
     "      kill -9 5678            强制终止PID为 5678 的进程\n"},
    {"ps",
     "ps: ps [选项]\n"
     "    报告当前进程状态。\n"
     "    显示当前正在运行的进程列表和状态信息。\n\n"
     "    例子:\n"
     "      ps                     显示当前正在运行的所有进程\n"
     "      ps -ef | grep bash     查找所有正在运行的 bash 进程\n"},
    {"history",
     "history: history [n]\n"
     "    显示命令历史记录。\n"
     "    显示命令历史记录列表。\n\n"
     "    选项:\n"
     "      n  显示最近的 n 条命令历史记录。\n\n"
     "    如果不提供参数，将显示所有历史记录。\n"
     "\n"
     "    例子:\n"
     "      history         显示所有历史记录。\n"
     "      history 10      显示最近的 10 条历史记录。\n"},
    {"type",
     "type: type [-a] name [name...]\n"
     "    显示命令类型。\n"
     "    显示指定命令的类型（别名、内置命令、可执行文件等）。\n\n"
     "    选项:\n"
     "      -a  显示所有匹配的命令路径，不仅仅是第一个匹配项。\n\n"
     "    如果命令存在多种类型（例如，既是别名又是内置命令），将显示所有匹配类型。\n"
     "\n"
     "    例子:\n"
     "      type ls        显示 'ls' 的类型。\n"
     "      type -a ls     显示所有匹配 'ls' 的路径。\n"},
    {"alias",
     "alias: alias [-p] [name[=value] ...]\n"
     "    创建或显示命令别名。\n"
     "    定义或显示别名。\n\n"
     "    如果没有参数，将显示所有已定义的别名。\n"
     "    如果指定了 name=value，将定义一个新的别名或更新现有的别名。\n"
     "    如果指定了 -p 选项，将显示所有已定义的别名，并按脚本可读的格式输出。\n"
     "\n"
     "    例子：\n"
     "      alias          显示所有别名。\n"
     "      alias ll='ls -la'  定义一个名为 'll' 的别名，其值为 'ls -la'。\n"
     "      alias -p      显示所有别名，并按脚本可读的格式输出。\n"}};

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
