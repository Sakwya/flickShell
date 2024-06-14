#include<builtins/alias.h>
#include<global.h>
#include<iostream>
#include<panic.h>

void alias(const std::vector<std::string>& args) {
  if (args.size() == 2 && args[1] == "-l") {
    // 显示所有别名
    if (alias_map.empty()) {
      std::cout << "No aliases defined." << std::endl;
    } else {
      for (const auto& pair : alias_map) {
        std::cout << pair.first << " = " << pair.second << std::endl;
      }
    }
  } else if (args.size() >= 3) {
    std::string subcommand = args[1];
    if (subcommand == "-d") {
      // 删除别名
      std::string alias_to_delete = args[2];
      auto it = alias_map.find(alias_to_delete);
      if (it != alias_map.end()) {
        alias_map.erase(it);
        std::cout << "Alias '" << alias_to_delete << "' deleted." << std::endl;
      } else {
        std::cout << "Alias '" << alias_to_delete << "' not found." << std::endl;
      }
    } else {
      // 添加别名
      std::string alias_name = subcommand;
      std::string command;
      for (size_t i = 2; i < args.size(); ++i) {
        command += args[i];
        if (i < args.size() - 1) {
          command += " ";
        }
      }
      alias_map[alias_name] = command;
      std::cout << "Alias '" << alias_name << "' added." << std::endl;
    }
  } else {
    // 错误使用 alias 命令
    panic("Usage: alias [-l] | alias <alias_name> <command> | alias -d <alias_name>");
  }
}
