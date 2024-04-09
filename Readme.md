## flickShell.cpp
> Origin: ExpShell.cpp
> A simple shell for Linux.  
> by z0gSh1u @ 2020-09  
> Revised By Sakwya @ 2024-03  

### 常见问题
#### 1.没有readline库。
Debian/Ubuntu 系统：sudo apt-get install libreadline-dev  
Fedora 系统：sudo dnf install readline-devel  
CentOS/RHEL 系统：sudo yum install readline-devel  
### 头文件
#### fcntl.h  
fcntl.h 是 C 语言中用于文件控制的头文件，其中定义了一些常量和函数原型，用于在程序中进行文件描述符的操作。  

常见的一些在 fcntl.h 中定义的常量包括：  

O_RDONLY：只读模式打开文件  
O_WRONLY：只写模式打开文件  
O_RDWR：读写模式打开文件  
O_CREAT：如果文件不存在则创建  
O_EXCL：如果与 O_CREAT 同时使用，文件若存在则返回错误  
O_TRUNC：如果文件存在并且成功以只写或者读写方式打开，则清空文件
O_APPEND：在每次写操作前移动到文件尾部  
O_NONBLOCK：以非阻塞模式打开文件  
等等  
此外，fcntl.h 中定义了一些函数原型，如 fcntl()、open() 等，用于对文件描述符进行各种操作。  
#### grp.h
grp.h 是 C 语言中用于组相关操作的头文件。在 Unix/Linux 系统中，每个用户都会属于一个或多个用户组（group），grp.h 提供了一些函数和结构体，用于获取和操作用户组信息。  

grp.h 中包含了一些函数和结构体，常见的有：  

struct group：用户组信息的结构体，包含用户组的名称、组ID、成员列表等信息。  
getgrnam()：根据用户组名称获取用户组信息。  
getgrgid()：根据用户组ID获取用户组信息。  
getgrent()：逐个读取用户组信息。  
setgrent()：将文件指针指向用户组文件开头。  
endgrent()：关闭用户组文件。  
#### sys/stat.h
sys/stat.h 是 C 语言中的头文件，用于文件状态的获取和操作。在 Unix/Linux 系统中，sys/stat.h 中包含了一些宏定义和结构体，用于获取文件的状态信息，比如文件大小、最后修改时间等。  

常见的宏定义和结构体包括：  

struct stat：文件状态的结构体，包含了文件的各种属性，如文件大小、权限、最后访问时间等。  
S_IFMT、S_IFREG、S_IFDIR 等：用于判断文件类型的宏定义。  
S_IRUSR、S_IWUSR、S_IXUSR 等：用于设置文件权限的宏定义。  
#### sys/types.h
sys/types.h 是 C 语言中的头文件，定义了一些基本的数据类型，如整数类型、指针类型等。这个头文件通常用于系统级编程，以确保程序在不同平台上具有一致的行为。  

常见的数据类型包括：  

size_t：无符号整数类型，用于表示对象大小。  
pid_t：进程 ID 类型，用于表示进程的唯一标识符。  
uid_t 和 gid_t：用户 ID 和组 ID 类型，用于表示用户和组的标识符。  
off_t：文件偏移量类型，用于表示文件中的偏移量。  
#### sys/wait.h
sys/wait.h 是 C 语言中的头文件，提供了一些用于处理子进程状态的函数和宏定义。在 Unix/Linux 系统中，sys/wait.h 中定义了一些函数和宏，用于等待子进程的状态变化并获取子进程的退出状态。  

常见的函数和宏包括：  

waitpid()：等待特定子进程的状态发生变化，并获取子进程的退出状态。  
wait()：等待任意子进程的状态发生变化，并获取子进程的退出状态。  
WIFEXITED(status)：用于判断子进程是否正常退出。  
WEXITSTATUS(status)：获取子进程正常退出时的退出状态。  
WIFSIGNALED(status)：用于判断子进程是否因信号而终止。  
#### unistd.h
unistd.h 是 C 语言中的头文件，提供了一些符号常量和函数原型，用于对 POSIX 操作系统（如 Unix/Linux）进行访问和控制。这个头文件中包含了许多与系统调用和文件操作相关的函数和常量。  

常见的函数和常量包括：  

文件操作函数：如 read()、write()、close() 等，用于文件的读写和关闭。  
进程控制函数：如 fork()、exec()、exit() 等，用于进程的创建、执行和退出。  
系统调用函数：如 syscall()，用于执行系统调用。  
符号常量：如 STDIN_FILENO、STDOUT_FILENO、STDERR_FILENO 等，用于表示标准输入、输出和错误文件描述符。  
