# Makefile

# 编译器
CXX = g++

# 编译器选项
CXXFLAGS = -Wall -g -lreadline -lstdc++

# 目标文件夹
BUILD_DIR = build

# 目标文件
TARGET = $(BUILD_DIR)/flickShell

# 源文件
SRCS = shell.cpp bashline.cpp eval.cpp line.cpp panic.cpp builtins/cd.cpp builtins/help.cpp builtins/history.cpp builtins/exit.cpp

# 生成的对象文件
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# 默认目标
all: $(TARGET)

# 链接对象文件生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CXXFLAGS)

# 编译.cpp文件为.o文件
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) -I. -c $< -o $@ $(CXXFLAGS)

# 创建目标文件夹
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 清理生成的文件
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

