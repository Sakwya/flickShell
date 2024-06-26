#include <bashline.h>
#include <builtins/builtins.h>
#include <eval.h>
#include <fcntl.h>
#include <global.h>
#include <line.h>  //包含了string 和 vector
#include <panic.h>
#include <shell_config.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>  //strcpy

using std::string;
using std::vector;

int fork_wrap() {
  int pid = fork();
  if (pid == -1) panic("fork failed.", true, 1);
  return pid;
}

// wrapped pipe function that panics
int pipe_wrap(int pipe_fd[2]) {
  int ret = pipe(pipe_fd);
  if (ret == -1) panic("pipe failed", true, 1);
  return ret;
}

// wrapped dup2 function that panics
int dup2_wrap(int fd1, int fd2) {
  int dup2_ret = dup2(fd1, fd2);
  if (dup2_ret < 0) panic("dup2 failed.", true, 1);
  return dup2_ret;
}

// wrapped open function that panics
int open_wrap(const char* file, int oflag) {
  int open_ret = open(file, oflag, 0666);  // 确保有权限参数
  if (open_ret < 0) panic("open failed.", true, 1);
  return open_ret;
}

// panic for wait status
int check_wait_status(int& wait_status) {
  if (WIFEXITED(wait_status) == 0) {  // means abnormal exit
    char buf[8];
    sprintf(buf, "%d", WEXITSTATUS(wait_status));
    if (SHOW_WAIT_PANIC) panic("child exit with code " + string(buf));
    return std::stoi(buf);
  }
  return 0;
}

// ==========================
// command line parsing
// ==========================

// base class for any cmd
class cmd {
 public:
  int type;
  cmd() { this->type = CMD_TYPE_NULL; }
};

// most common type of cmd
// argv[0] ...argv[1~n]
class exec_cmd : public cmd {
 public:
  vector<string> argv;
  exec_cmd(vector<string>& argv) {
    this->type = CMD_TYPE_EXEC;
    this->argv = vector<string>(argv);
  }
};

// pipe cmd
// left | right
class pipe_cmd : public cmd {
 public:
  cmd* left;
  cmd* right;
  pipe_cmd() { this->type = CMD_TYPE_PIPE; }
  pipe_cmd(cmd* left, cmd* right) {
    this->type = CMD_TYPE_PIPE;
    this->left = left;
    this->right = right;
  }
};

// redirect cmd
// ls > a.txt; some_program < b.txt
class redirect_cmd : public cmd {
 public:
  cmd* cmd_;
  string file;
  int fd;
  redirect_cmd() {}
  redirect_cmd(int type, cmd* cmd_, string file, int fd) {
    this->type = type;
    this->cmd_ = cmd_;
    this->file = file;
    this->fd = fd;
  }
};
class and_cmd : public cmd {
 public:
  cmd* cmd_;
  cmd* rcmd;
  int background = CMD_BACKGROUND_FALSE;
  and_cmd() { this->type = CMD_TYPE_AND; }
  and_cmd(cmd* cmd_, cmd* rcmd) {
    this->type = CMD_TYPE_AND;
    this->cmd_ = cmd_;
    this->rcmd = rcmd;
  }
};
class background_cmd : public and_cmd {
 public:
  background_cmd() : and_cmd() { this->background = CMD_BACKGROUND_TRUE; }
  background_cmd(cmd* cmd_, cmd* rcmd) : and_cmd(cmd_, rcmd) {
    this->background = CMD_BACKGROUND_TRUE;
  }
};

// parse seg as is exec_cmd
cmd* parse_exec_cmd(string seg) {
  seg = trim(seg);
  vector<string> argv = string_split_protect(seg, WHITE_SPACE);
  return new exec_cmd(argv);
}

// divide-and-conquer
// **test cases:**
// ls -a < a.txt | grep linux > b.txt
// some_bin "hello world" > b.txt > c.txt
cmd* parse(string line) {
  line = trim(line);
  string cur_read = "";
  cmd* cur_cmd = new cmd();
  int i = 0;
  while (i < line.length()) {
    if (line[i] == '<' || line[i] == '>') {
      cmd* lhs = parse_exec_cmd(cur_read);  // [lhs] < (or >) [rhs]
      int j = i + 1;
      while (j < line.length() && !is_symbol(line[j])) j++;
      string file = trim(line.substr(i + 1, j - i));
      cur_cmd = new redirect_cmd(
          line[i] == '<' ? CMD_TYPE_REDIR_IN : CMD_TYPE_REDIR_OUT, lhs, file,
          -1);  // fd wait for filling
      i = j;
    } else if (line[i] == '|') {
      cmd* rhs = parse(line.substr(i + 1));  // recursive
      if (cur_cmd->type == CMD_TYPE_NULL) cur_cmd = parse_exec_cmd(cur_read);
      cur_cmd = new pipe_cmd(cur_cmd, rhs);
      return cur_cmd;
    } else if (line[i] == '&') {
      if (i + 1 < line.length() && line[i + 1] == '&') {
        cmd* rhs = parse(line.substr(i + 2));  // recursive
        if (cur_cmd->type == CMD_TYPE_NULL) cur_cmd = parse_exec_cmd(cur_read);
        cur_cmd = new and_cmd(cur_cmd, rhs);
        return cur_cmd;
      }
      cmd* rhs = parse(line.substr(i + 1));  // recursive
      if (cur_cmd->type == CMD_TYPE_NULL) cur_cmd = parse_exec_cmd(cur_read);
      cur_cmd = new background_cmd(cur_cmd, rhs);
      return cur_cmd;
    } else
      cur_read += line[i++];
  }
  if (cur_cmd->type == CMD_TYPE_NULL)
    return parse_exec_cmd(cur_read);
  else
    return cur_cmd;
}

// run some cmd
int run_cmd(cmd* cmd_, int c_index = 1) {
  int exit_code = 0;
  switch (cmd_->type) {
    case CMD_TYPE_EXEC: {
      exec_cmd* ecmd = static_cast<exec_cmd*>(cmd_);
      // 处理别名
      if (alias_map.count(ecmd->argv[0]) != 0) {
        vector<string> alias_args =
            string_split(alias_map[ecmd->argv[0]], WHITE_SPACE);
        ecmd->argv.erase(ecmd->argv.begin());  // 移除别名
        ecmd->argv.insert(ecmd->argv.begin(), alias_args.begin(),
                          alias_args.end());
      }
      // 检查并执行内置命令
      if (is_builtin(ecmd->argv[0])) {
        run_builtin(ecmd->argv[0], ecmd->argv);
      } else {
        // 准备并执行外部命令
        vector<char*> argv_c_str;
        for (const auto& arg : ecmd->argv) {
          argv_c_str.push_back(const_cast<char*>(arg.c_str()));
        }
        argv_c_str.push_back(nullptr);

        if (execvp(argv_c_str[0], argv_c_str.data()) < 0) {
          panic("execvp failed", true, errno);
        }
      }
      return exit_code;
    }

    case CMD_TYPE_PIPE: {
      pipe_cmd* pcmd = static_cast<pipe_cmd*>(cmd_);
      int pipe_fd[2];
      pipe_wrap(pipe_fd);
      // fork twice to run lhs and rhs of pipe
      if (fork_wrap() == 0) {
        // i'm a child, let's satisfy lhs
        close(pipe_fd[0]);
        dup2_wrap(pipe_fd[1], fileno(stdout));  // lhs_stdout -> pipe_write
        // close the original ones
        run_cmd(pcmd->left);
        close(pipe_fd[1]);
      }
      if (fork_wrap() == 0) {
        // i'm also a child, let's satisfy rhs
        close(pipe_fd[1]);
        dup2_wrap(pipe_fd[0], fileno(stdin));  // pipe_read -> rhs_stdin
        run_cmd(pcmd->right);
        close(pipe_fd[0]);
      }
      // really good. now we have lhs_stdout -> pipe -> rhs_stdin
      // if fork > 0, then i'm the father
      // let's wait for my children
      // close()会等待IO操作。
      close(pipe_fd[0]);
      close(pipe_fd[1]);
      // 等待两个子进程退出
      int wait_status_1, wait_status_2;
      wait(&wait_status_1);
      wait(&wait_status_2);
      // 打印子进程的退出信息
      exit_code = check_wait_status(wait_status_1);
      if (exit_code != 0) return exit_code;
      exit_code = check_wait_status(wait_status_2);
      return exit_code;
    }
    case CMD_TYPE_REDIR_IN:
      // 可以在这里初始化flag
    case CMD_TYPE_REDIR_OUT: {
      redirect_cmd* rcmd = static_cast<redirect_cmd*>(cmd_);
      if (fork_wrap() == 0) {
        // i'm a child, let's satisfy the file being redirected to (or from)
        if (rcmd->type == CMD_TYPE_REDIR_IN) {
          rcmd->fd = open_wrap(rcmd->file.c_str(), REDIR_IN_OFLAG);
          dup2_wrap(rcmd->fd, STDIN_FILENO);
        } else {
          rcmd->fd = open_wrap(rcmd->file.c_str(), REDIR_OUT_OFLAG);
          dup2_wrap(rcmd->fd, STDOUT_FILENO);
        }
        close(rcmd->fd);
        run_cmd(rcmd->cmd_);
        exit(0);
      }
      // if fork > 0, then i'm the father
      // let's wait for my children
      int wait_status;
      wait(&wait_status);
      exit_code = check_wait_status(wait_status);
      return exit_code;
    }
    case CMD_TYPE_AND: {
      and_cmd* acmd = static_cast<and_cmd*>(cmd_);
      if (fork_wrap() == 0) {
        if (acmd->background == CMD_BACKGROUND_TRUE) {
          int child_pid;
          if ((child_pid = fork_wrap()) == 0) {
            // i'm a child, let's run lhs;
            run_cmd(acmd->cmd_, c_index);
          } else {
            std::string message = "[";
            message.append(std::to_string(c_index));
            message.append("] ");
            message.append(std::to_string(child_pid));
            panic(message, true, 0);
          }
        } else {
          exit_code = run_cmd(acmd->cmd_);
        }
      }
      // if fork > 0, then i'm the father
      // let's wait for my children
      int wait_status;
      wait(&wait_status);
      exit_code = check_wait_status(wait_status);

      if (exit_code != 0) return exit_code;
      if (acmd->background == CMD_BACKGROUND_TRUE) c_index += 1;
      exit_code = run_cmd(acmd->rcmd, c_index);
      return exit_code;
    }
    default:
      panic("unknown or null cmd type", true, 1);
  }
  return exit_code;
}

void processing_rc() {
  set_prompt();
  int wait_status;
  for (auto& line : flickshrc) {
    line = trim(line);
    if (line.empty()) continue;
    if (line[0] == '#') continue;
    // deal with builtin commands
    std::vector<std::string> args = string_split_protect(line, WHITE_SPACE);
    if (is_builtin(args[0])) {
      run_builtin(args[0], args);
      continue;
    }
    // fork a new me to execute the typed command
    if (fork_wrap() == 0) {
      cmd* cmd_ = parse(line);
      run_cmd(cmd_);
      exit(0);  // child exit
    }
    wait(&wait_status);
    check_wait_status(wait_status);
  }
}

void reader_loop() {
  set_prompt();
  string line;
  int wait_status;
  while (true) {
    line = trim(read_line());
    if (line.empty()) continue;
    if (line[0] == '#') continue;
    // deal with builtin commands
    std::vector<std::string> args = string_split_protect(line, WHITE_SPACE);
    if (is_builtin(args[0])) {
      run_builtin(args[0], args);
      continue;
    }
    // fork a new me to execute the typed command
    if (fork_wrap() == 0) {
      cmd* cmd_ = parse(line);
      run_cmd(cmd_);
      exit(0);  // child exit
    }
    wait(&wait_status);
    check_wait_status(wait_status);
  }
}