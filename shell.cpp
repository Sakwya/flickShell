#include <bashline.h>
#include <eval.h>
#include <fstream>
#include <global.h>
#include <panic.h>
#include <readline/history.h>
#include <readline/readline.h>
using namespace std;

void init_shell() {
  get_command_prompt();
  string filePath = home_dir + "/.flickshrc";
  panic(filePath);
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
  }
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
