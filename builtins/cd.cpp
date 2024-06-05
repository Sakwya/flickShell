// cd.cpp
#include "eval.h"
#include <unistd.h>

namespace builtins {

int cd(const string& line) {
    // 解析命令行参数
    vector<string> args = string_split(line, WHITE_SPACE);

    // 检查命令参数数量
    if (args.size() < 2) {
        // 如果没有提供参数，默认切换到用户的家目录
        chdir(getenv("HOME"));
    } else {
        // 如果有参数，尝试切换到指定的目录
        string dir = args[1];
        if (dir.front() == '\~') {
            // 如果目录以"\~"开头，则将其转换为绝对路径
            dir = getenv("HOME") + dir.substr(1);
        }
        if (chdir(dir.c_str()) < 0) {
            // 如果切换目录失败，则打印错误信息
            cerr << "cd: cannot change directory to '" << dir << "'" << endl;
            return -1;
        }
    }

    // 更新提示符
    prompt = get_command_prompt();

    // 返回成功标志
    return 0;
}

} // namespace builtins
