#ifndef LINE_H
#define LINE_H
#include<string>
using std::string;
#include <vector>
using std::vector;

const string WHITE_SPACE = " \t\r\n";
const string SYMBOL = "|<>";

extern bool is_white_space(char);
extern bool is_symbol(char);
extern vector<string> string_split(const string&, const string&);
extern vector<string> string_split_protect(const string&, const string&);
extern string string_split_last(const string&, const string&);
extern string string_split_first(const string&, const string&);
extern string trim(const string&);

#endif //LINE_H