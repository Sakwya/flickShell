// history.cpp
#include "eval.h"
#include <vector>

namespace builtins {

int history(const string& line) {
    // 默认情况下，只显示当前历史记录
    if (line == "history") {
        for (int i = history_base; i < history_length; ++i) {
            HIST_ENTRY* entry = history_get(i);
            if (entry) {
                cout << i + 1 << ": " << entry->line << endl;
            }
        }
        return 0;
    }

    // 如果提供了参数，尝试显示指定编号的历史记录
    vector<string> args = string_split(line, WHITE_SPACE);
    if (args.size() > 1) {
        int index = stoi(args[1]);
        if (index >= 1 && index <= history_length) {
            HIST_ENTRY* entry = history_get(index - 1);
            if (entry) {
                cout << index << ": " << entry->line << endl;
            } else {
                cout << "No such history entry." << endl;
            }
        } else {
            cout << "Invalid history index." << endl;
        }
        return 0;
    }

    // 如果参数不正确，返回错误
    cout << "Invalid history command." << endl;
    return -1;
}

} // namespace builtins
