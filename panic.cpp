#include <panic.h>
#include <shell_config.h>

#include <iostream>
void panic(const std::string& hint, bool exit_, int exit_code) {
  if (SHOW_PANIC) std::cerr << "[!flickShell]: " << hint << std::endl;
  if (exit_) exit(exit_code);
}
void message(const std::string& hint) { std::cout << hint << std::endl; }