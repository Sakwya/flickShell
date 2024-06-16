#include <builtins/builtins.h>
#include <builtins/type.h>
#include <global.h>
#include <limits.h>
#include <panic.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <map>

// 获取文件的绝对路径
std::string get_absolute_path(const std::string& path) {
  char abs_path[PATH_MAX];
  if (realpath(path.c_str(), abs_path) != nullptr) {
    return std::string(abs_path);
  } else {
    return path;  // 如果无法获取绝对路径，则返回原始路径
  }
}

enum CommandType { NOT_FOUND, BUILTIN, ALIAS, EXEC_COMMAND };

CommandType get_command_type(const std::string& name) {
  // 检查是否是内置命令
  if (is_builtin(name)) {
    return BUILTIN;
  }
  // 检查是否是别名
  if (alias_map.find(name) != alias_map.end()) {
    return ALIAS;
  }
  // 检查是否是可执行命令
  if (command_map.find(name) != command_map.end()) {
    return EXEC_COMMAND;
  }
  return NOT_FOUND;
}

void show_type(const std::string& command_name) {
  struct stat file_stat;
  std::string command_path;

  // 检查是否是内置命令或别名
  CommandType commandType = get_command_type(command_name);
  switch (commandType) {
    case BUILTIN:
      std::cout << command_name << " is builtin command" << std::endl;
      break;
    case ALIAS:
      std::cout << command_name << " is aliased to `"
                << alias_map.at(command_name) << '`' << std::endl;
      break;
    case EXEC_COMMAND:
      std::cout << command_name << " is " << command_map.at(command_name)
                << std::endl;
      break;
    default:
      std::cout << "command not found: " << command_name << std::endl;
      break;
  }
}
void display_type(const std::vector<std::string>& args) {
  // 检查参数数量
  size_t index = 1, size = args.size();
  while (index < size) {
    show_type(args[index]);
    index++;
  }
  return;
}
