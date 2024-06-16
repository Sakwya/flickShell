#include <bashline.h>
#include <config.h>
#include <global.h>
#include <line.h>
#include <panic.h>
#include <pwd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>

// #include <fstream>

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