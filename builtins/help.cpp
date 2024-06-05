// help.cpp
#include "eval.h"
#include <unordered_map>

namespace builtins {

int help(const string& line) {
    // 如果只有一个参数，显示所有帮助主题
    if (line == "help") {
        cout << "Available commands:" << endl;
        for (const auto& entry : help_map) {
            cout << "  " << entry.first << ": " << entry.second << endl;
        }
        return 0;
    }

    // 如果有多个参数，尝试查找特定的帮助主题
    vector<string> args = string_split(line, WHITE_SPACE);
    if (args.size() > 1) {
        string key = args[1];
        try {
            string value = help_map.at(key);
            cout << value << endl;
        } catch (const std::out_of_range& e) {
            cout << "No help topics match `" << key << "'." << endl;
        }
        return 0;
    }

    // 如果参数不正确，返回错误
    cout << "Invalid help command." << endl;
    return -1;
}

} // namespace builtins
