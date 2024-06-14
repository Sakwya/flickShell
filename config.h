#define SHOW_PANIC true
#define SHOW_WAIT_PANIC false
#define CHAR_BUF_SIZE 1024
#define MAX_ARGV_LEN 128

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"

#define CMD_TYPE_NULL 0      // initial value
#define CMD_TYPE_EXEC 1      // common exec command
#define CMD_TYPE_PIPE 2      // pipe command
#define CMD_TYPE_REDIR_IN 4  // redirect using <
#define CMD_TYPE_REDIR_OUT 8 // redirect using >
#define REDIR_IN_OFLAG O_RDONLY
#define REDIR_OUT_OFLAG O_WRONLY | O_CREAT | O_TRUNC