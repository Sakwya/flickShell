#include<bashline.h>

#define CMD_TYPE_NULL 0      // initial value
#define CMD_TYPE_EXEC 1      // common exec command
#define CMD_TYPE_PIPE 2      // pipe command
#define CMD_TYPE_REDIR_IN 4  // redirect using <
#define CMD_TYPE_REDIR_OUT 8 // redirect using >
#define REDIR_IN_OFLAG O_RDONLY
#define REDIR_OUT_OFLAG O_WRONLY | O_CREAT | O_TRUNC


#include <fcntl.h>
#include <map>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

extern void reader_loop();


#include <cstdlib>
#include <cstring>
#include <grp.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>