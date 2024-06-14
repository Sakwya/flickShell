#include <bashline.h>
#include <eval.h>
#include <fstream>
#include <global.h>
#include <panic.h>
#include <readline/history.h>
#include <readline/readline.h>
using namespace std;

void init_shell() {
  set_prompt();
  string filePath = home_dir + "/.flickshrc";
  std::ifstream usrConfig(filePath);
  if (!usrConfig.good()) {
    usrConfig.close();
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
  while (std::getline(usrConfig, line)) {
    // 在这里处理每一行的内容，比如打印出来
    if (line[0]=='#'){

    }
  }
  usrConfig.close();
}

int main() {
  rl_initialize();
  using_history();
  fflush(stdout);
  rl_on_new_line();
  rl_bind_key('\t', rl_complete);

  init_shell();
  reader_loop();
  return 0;
}
