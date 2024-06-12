#include <bashline.h>
#include <line.h>
#include <unistd.h>
#include <pwd.h>
#include <fstream>
#include <global.h>
#include <config.h>
#include <readline/readline.h>
#include <readline/history.h>
// ==========================
// show the command prompt in front of each line
// **example** [root@localhost tmp]>
// ==========================

uid_t uid = -1;
passwd* pwd;
string username;
string hostname;

char char_buf[CHAR_BUF_SIZE];

string get_command_prompt() {
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
      home_dir = "/root"; // home for root
    else {
      home_dir = "/home/";
      home_dir.append(username); // home for other user
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

  //prompt = getenv("P1");

  // output
  string _prompt = "[";
  _prompt.append(username);
  _prompt.push_back('@');
  _prompt.append(hostname);
  _prompt.push_back(' ');
  _prompt.append(cwd);
  _prompt.append("]>");
  return _prompt;
}

string read_line(){
  string line = trim(readline(prompt.c_str()));
  while (line.empty())
  {
    line = trim(readline(prompt.c_str()));
  }
  add_history(line.c_str());
  return line;
}