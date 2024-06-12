#include "builtin_type.h"
#include "eval.h"  // 包含eval.h以获取panic函数的声明
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>

void show_type(const std::vector<std::string>& args) {
    // 检查参数数量
    if (args.size() != 2) {
        panic("invalid number of arguments");
    }

    // 获取要显示类型的命令或文件名
    const std::string& command_or_file = args[1];

    struct stat file_stat;
    if (stat(command_or_file.c_str(), &file_stat) == 0) {
        // 如果stat调用成功，我们可以检查文件类型
        if (S_ISDIR(file_stat.st_mode)) {
            std::cout << "directory" << std::endl;
        } else if (S_ISREG(file_stat.st_mode)) {
            std::cout << "regular file" << std::endl;
        } else if (S_ISLNK(file_stat.st_mode)) {
            std::cout << "symlink" << std::endl;
        } else if (S_ISCHR(file_stat.st_mode)) {
            std::cout << "character special device" << std::endl;
        } else if (S_ISBLK(file_stat.st_mode)) {
            std::cout << "block special device" << std::endl;
        } else if (S_ISFIFO(file_stat.st_mode)) {
            std::cout << "fifo" << std::endl;
        } else if (S_ISSOCK(file_stat.st_mode)) {
            std::cout << "socket" << std::endl;
        } else {
            std::cout << "unknown type" << std::endl;
        }
    } else {
        // stat调用失败，可能是因为文件不存在
        std::cout << "file does not exist or cannot be accessed" << std::endl;
    }
}
