// exit.cpp
#include "eval.h"

namespace builtins {

int exit(const string& line) {
    // 如果只有一个参数，退出程序
    if (line == "exit") {
        cout << "Bye from flickShell." << endl;
        exit(0);
    }

    // 如果有多个参数，尝试退出程序并返回状态码
    vector<string> args = string_split(line, WHITE_SPACE);
    if (args.size() > 1) {
        try {
            int status = stoi(args[1]);
            exit(status);
        } catch (const std::invalid_argument& e) {
            cout << "Invalid exit status." << endl;
        }
        return 0;
    }

    // 如果参数不正确，返回错误
    cout << "Invalid exit command." << endl;
    return -1;
}

} // namespace builtins
