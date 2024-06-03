#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"

#include<panic.h>

using string = std::string;
using std::endl;
using std::cerr;

#include<config.h>

const string WHITE_SPACE = " \t\r\n";
const string SYMBOL = "|<>";

#include <vector>
using std::vector;

extern bool is_white_space(char);
extern bool is_symbol(char);
extern vector<string> string_split(const string&, const string&);
extern vector<string> string_split_protect(const string&, const string&);
extern string string_split_last(const string&, const string&);
extern string string_split_first(const string&, const string&);
extern string trim(const string&);
