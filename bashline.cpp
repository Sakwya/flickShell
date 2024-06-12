#include <bashline.h>

// ==========================
// show the command prompt in front of each line
// **example** [root@localhost tmp]>
// ==========================

uid_t uid = -1;
passwd* pwd;
string username;
string hostname;
string home_dir;
char char_buf[CHAR_BUF_SIZE];
string get_command_prompt();

void init_shell() {
  get_command_prompt();
  string filePath = home_dir + "/.flickshrc";
  panic(filePath);
  std::ifstream usrConfig(filePath);
  if (!usrConfig.good()) {
    usrConfig.close();
    std::ofstream newConfig(filePath, std::ios::app);

    if (!newConfig.is_open()) {
      panic("Failed to open file: "+filePath);
      exit(1);
    }else{
      newConfig << "# ./flickshrc"<<endl;
    }
  }
}

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
  string prompt = "[";
  prompt.append(username);
  prompt.push_back('@');
  prompt.append(hostname);
  prompt.push_back(' ');
  prompt.append(cwd);
  prompt.append("]>");
  return prompt;
}