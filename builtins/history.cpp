#include <builtins/history.h>
#include <readline/history.h>

void show_history(const std::vector<std::string>& args) {

    for (int i = history_base; i < history_length; ++i) {
        // 获取第i条历史记录
        HIST_ENTRY* history_entry = history_get(i);
        // 输出历史记录
        if (history_entry != NULL) {
            printf("%5d  %s\n", i + 1, history_entry->line);
        }
    }
}
