#include <bashline.h>
#include <eval.h>
#include <fstream>
#include <global.h>
#include <panic.h>
#include <readline/history.h>
#include <readline/readline.h>

#include <iostream>

using namespace std;

// 命令列表
const char* shell_commands[] = {
    "help", "exit", "ls", "cd",
    "cat", "grep", "pwd",
    "history", "type", "alias",
    NULL
};

// 命令生成器
char* command_generator(const char* text, int state) {
    static int list_index = 0;
    static const char *name;
    size_t len = strlen(text);

    if (!state) {
        list_index = 0; // 重置索引
    }

    while (shell_commands[list_index]) {
        name = shell_commands[list_index++];
        if (strncmp(name, text, len) == 0) {
            return strdup(name); // 返回匹配的命令
        }
    }
    return NULL; // 未找到匹配的命令
}

// 补全逻辑
char** shell_completion(const char* text, int start, int end) {
    if (start == 0) {
        return rl_completion_matches(text, command_generator);
    }
    return NULL;
}

// 初始化readline
void init_readline() {
    rl_readline_name = "FlickShell"; // 设置shell的名字
    rl_attempted_completion_function = shell_completion; // 设置补全函数
}

// 初始化shell配置
void init_shell() {
  set_prompt();
  string filePath = home_dir + "/.flickshrc";
  std::ifstream rc_flie(filePath);
  if (!rc_flie.good()) {
    rc_flie.close();
    std::ofstream newConfig(filePath, std::ios::app);
    if (!newConfig.is_open()) {
      panic("Failed to open file: " + filePath, true, 1);
    } else {
      newConfig << "# ./flickshrc" << endl;
      newConfig.close();
    }
    return;
  }
  std::string line;
  while (std::getline(rc_flie, line)) {
    // 在这里处理每一行的内容，比如打印出来
    flickshrc.push_back(line);
  }
  rc_flie.close();
}

int main() {
  rl_initialize();
  init_readline();
  using_history();
  fflush(stdout);
  rl_on_new_line();
  rl_bind_key('\t', rl_complete);
  init_shell();
  processing_rc();
  reader_loop();
  return 0;
}
