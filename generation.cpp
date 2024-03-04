#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    int fdShell, fdTemplate, fdInstall;
    fdShell = open("flickShell.cpp", O_RDONLY, 0644);
    fdInstall = open("install_shell.sh", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdShell == -1 || fdInstall == -1) {
        std::cerr << "无法打开文件。请确保flickShell.cpp在当前目录中, 且用户具有读写权限。" << std::endl;
        return 1;
    }
    fdTemplate = open("install_shell.template", O_RDONLY, 0644);
    if (fdTemplate == -1) {
        std::cerr << "无法打开文件。请确保install_shell.template在当前目录中, 且用户具有读写权限。" << std::endl;
        return 1;
    }
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    std::string shell_content, template_content;
    while ((bytes_read = read(fdShell, buffer, BUFFER_SIZE)) > 0) {
        shell_content.append(buffer, bytes_read);
    }
    if (bytes_read == -1) {
        std::cerr << "读取文件时发生错误" << std::endl;
        return 1;
    }
    while ((bytes_read = read(fdTemplate, buffer, BUFFER_SIZE)) > 0) {
        template_content.append(buffer, bytes_read);
    }
    if (bytes_read == -1) {
        std::cerr << "读取文件时发生错误" << std::endl;
        return 1;
    }
    template_content.replace(template_content.find("{{cpp}}"), 7, shell_content);


    ssize_t bytes_written = write(fdInstall, template_content.c_str(), template_content.length());

    if (bytes_written == -1) {
        std::cerr << "写入文件时发生错误" << std::endl;
        return 1;
    }

    std::cout << "成功写入 " << bytes_written << " 字节到文件" << std::endl;

    return 0;
}