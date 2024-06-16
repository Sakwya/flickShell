cmake_minimum_required(VERSION 3.5)
project(flickShell)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# # 查找需要的库（例如：readline）
# find_package(Readline REQUIRED)

# 添加源文件列表
set(SOURCES
    shell.cpp
    bashline.cpp
    eval.cpp
    line.cpp
    panic.cpp
    global.cpp
    builtins/alias.cpp
    builtins/builtins.cpp
    builtins/cd.cpp
    builtins/help.cpp
    builtins/history.cpp
    builtins/type.cpp
    builtins/export.cpp
    builtins/exit.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加可执行文件目标
add_executable(flickShell ${SOURCES})

set(Readline_INCLUDE_DIRS "/usr/include/readline")
set(Readline_LIBRARIES "-lreadline")

# find_package(Readline REQUIRED)
# 链接 readline 库
target_link_libraries(flickShell PRIVATE ${Readline_LIBRARIES})

# 设置编译选项
target_compile_options(flickShell PRIVATE -Wall -g)

# 指定包含的目录
target_include_directories(flickShell PRIVATE
    ${Readline_INCLUDE_DIR}
)
