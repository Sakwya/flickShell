#include <builtins/history.h>
#include <panic.h>
#include <readline/history.h>

#include <stdexcept>
void display_history(const std::vector<std::string>& args) {
  int history_start;
  switch (args.size()) {
    case 1:
      history_start = history_base;
      break;
    case 2:
      try {
        int history_num = std::stoi(args[1]);
        history_start = history_length - history_num;
        history_start =
            (history_start > history_base) ? history_start + 1 : history_base;
      } catch (std::invalid_argument) {
        panic("numeric argument required");
        return;
      }
      break;
    default:
      panic("too many arguments");
      return;
  }
  for (; history_start <= history_length; ++history_start) {
    // 获取第i条历史记录
    HIST_ENTRY* history_entry = history_get(history_start);
    // 输出历史记录
    if (history_entry != NULL) {
      printf("%5d  %s\n", history_start, history_entry->line);
    }
  }
  return;
}
