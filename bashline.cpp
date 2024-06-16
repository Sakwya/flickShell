#include <bashline.h>
#include <config.h>
#include <global.h>
#include <line.h>
#include <panic.h>
#include <pwd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>

#include <filesystem>
namespace fs = std::filesystem;

// ==========================
// show the command prompt in front of each line
// **example** [root@localhost tmp]>
// ==========================

uid_t uid = -1;
passwd* pwd;
string username;
string hostname;

char char_buf[CHAR_BUF_SIZE];

void set_prompt() {
  uid_t new_uid = getuid();
  if (new_uid != uid) {
    uid = new_uid;
    pwd = getpwuid(uid);
    username = (pwd->pw_name);
    gethostname(char_buf, CHAR_BUF_SIZE);
    hostname = char_buf;
    // sometimes, hostname is like localhost.locald.xxx here, should split it
    hostname = string_split_first(hostname, ".");

    // consider home path (~)
    if (username == "root")
      home_dir = "/root";  // home for root
    else {
      home_dir = "/home/";
      home_dir.append(username);  // home for other user
    }
  }

  // get current working directory
  getcwd(char_buf, CHAR_BUF_SIZE);
  string cwd(char_buf);

  if (cwd == home_dir)
    cwd = "~";
  else if (cwd != "/") {
    // consider root path (/)
    // keep only the last level of directory
    cwd = string_split_last(cwd, "/");
  }
  // char* P1 = getenv("P1");
  // if (P1) {
  //   prompt.assign(P1);
  // } else {
  //   panic("空指针");
  // }
  prompt.clear();
  if (username != "root") {
    prompt.append(ANSI_COLOR_CYAN);
    prompt = "[";
    prompt.append(ANSI_COLOR_GREEN);
    prompt.append(username);
    prompt.push_back('@');
    prompt.append(hostname);
    prompt.append(ANSI_COLOR_CYAN);
    prompt.push_back(' ');
    prompt.append(cwd);
    prompt.append(ANSI_COLOR_RESET);
    prompt.append("]> ");
    prompt.append(ANSI_COLOR_RESET);
  } else {
    prompt = "[";
    prompt.append(username);
    prompt.push_back('@');
    prompt.append(hostname);
    prompt.push_back(' ');
    prompt.append(cwd);
    prompt.append("]> ");
  }
  return;
}

string read_line() {
  string line = trim(readline(prompt.c_str()));
  while (line.empty()) {
    line = trim(readline(prompt.c_str()));
  }
  add_history(line.c_str());
  return line;
}

// ==========================
// auto complete
// ==========================

void populateCommandMap() {
  // 获取 PATH 环境变量
  const char* path = std::getenv("PATH");
  if (path == nullptr) {
    panic("PATH environment variable is not set.");
    return;
  }

  // 将 PATH 转换为字符串向量
  std::vector<std::string> pathList;
  std::string pathStr(path);
  size_t start = 0, end;
  while ((end = pathStr.find(":", start)) != std::string::npos) {
    pathList.push_back(pathStr.substr(start, end - start));
    start = end + 1;
  }
  pathList.push_back(pathStr.substr(start));

  // 遍历每个路径并将可执行文件加入映射
  for (const auto& dir : pathList) {
    try {
      for (const auto& entry : fs::directory_iterator(dir)) {
        if (fs::is_regular_file(entry) &&
            (fs::status(entry).permissions() & fs::perms::owner_exec) !=
                fs::perms::none) {
          // 获取文件名作为命令，完整路径作为值
          std::string command = entry.path().filename().string();
          std::string fullPath = entry.path().string();
          command_map[command] = fullPath;
        }
      }
    } catch (const fs::filesystem_error& e) {
      std::string message = "path cannot access: ";
      message.append(dir);
      panic(message);
      continue;
    }
  }
}
// 命令生成器
char* command_generator(const char* text, int state) {
  static std::vector<std::string> matches;
  static size_t match_index;
  size_t len = strlen(text);

  if (!state) {
    matches.clear();
    match_index = 0;

    // 查找 shell_commands

    for (const auto& name : builtins_set) {
      if (name.compare(0, len, text) == 0) {
        matches.push_back(name);
      }
    }

    for (const auto& pair : alias_map) {
      if (pair.first.compare(0, len, text) == 0) {
        matches.push_back(pair.first);
      }
    }

    // 查找 command_map
    for (const auto& pair : command_map) {
      if (pair.first.compare(0, len, text) == 0) {
        matches.push_back(pair.first);
      }
    }
  }

  if (match_index < matches.size()) {
    return strdup(matches[match_index++].c_str());
  } else {
    return NULL;
  }
}

// 补全逻辑
char** shell_completion(const char* text, int start, int end) {
  if (start == 0) {
    return rl_completion_matches(text, command_generator);
  }
  return NULL;
}

void init_bashline() {
  populateCommandMap();
  rl_initialize();
  using_history();
  fflush(stdout);
  rl_on_new_line();
  rl_bind_key('\t', rl_complete);
  rl_readline_name = "flickShell";                      // 设置shell的名字
  rl_attempted_completion_function = shell_completion;  // 设置补全函数
}