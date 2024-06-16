#include <builtins/alias.h>
#include <global.h>
#include <panic.h>

#include <iostream>
void show_alias_map() {
  if (alias_map.empty())
    std::cout << "No aliases defined." << std::endl;
  else
    for (const auto& pair : alias_map)
      std::cout << "alias " << pair.first << "='" << pair.second << '\''
                << std::endl;
  return;
}
void parse_alias(const std::string& str) {
  int index = 0;
  if (str[0] == '=' || str[0] == '-') {
    std::string message = "invalid name: ";
    message.append(str);
    panic(message);
    return;
  }
  for (int i = 1; i < str.length(); i++) {
    if (str[i] == '=') {
      std::string name = str.substr(0, i);
      std::string value = str.substr(i + 1);
      alias_map[name] = value;
      return;
    }
  }
  auto it = alias_map.find(str);
  if (it != alias_map.end()) {
    std::cout << "alias " << str << "='" << it->second << '\'' << std::endl;
  }
}
void alias(const std::vector<std::string>& args) {
  switch (args.size()) {
    case 1:
      show_alias_map();
      return;
    case 2:
      if (args[1] == "-p") {
        show_alias_map();
        return;
      }
      parse_alias(args[1]);
    default:
      break;
  }
  // size > 2
  int index = 1;
  if (args[1] == "-p") {
    show_alias_map();
    index = 2;
  }
  for (; index < args.size(); index++) parse_alias(args[index]);
  return;
}

extern void unalias(const std::vector<std::string>& args) {
  if (args.size() == 1) {
    message("unalias: usage: unalias [-a] name [name ...]");
  }
  if (args[1] == "-a") {
    alias_map.clear();
    return;
  }
  for (int i = 1; i < args.size(); i++) {
    auto it = alias_map.find(args[i]);
    if (it != alias_map.end()) {
      alias_map.erase(args[i]);
      continue;
    }
    std::string message = "unalias: ";
    message.append(args[i]);
    message.append(": not found");
    panic(message);
  }
}