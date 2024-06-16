// 用于测试输入输出
#include <iostream>

int main(int argc, char *argv[]) {
  std::string line;
  while (std::getline(std::cin, line)) {
    std::cout << "get:" << line << std::endl;
  }
  return 0;
}