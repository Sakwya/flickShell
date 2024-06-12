#include<panic.h>
#include<iostream>
#include<config.h>
void panic(std::string hint, bool exit_, int exit_code) {
  if (SHOW_PANIC)
    std::cerr << "[!flickShell]: " << hint << std::endl;
  if (exit_)
    exit(exit_code);
}