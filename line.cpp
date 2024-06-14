#include <config.h>
#include <line.h>
#include <panic.h>

bool is_white_space(char ch) { return WHITE_SPACE.find(ch) != -1; }

bool is_symbol(char ch) { return SYMBOL.find(ch) != -1; }

vector<string> string_split(const string& s, const string& delims) {
  vector<string> vec;
  int p = 0, q;
  while ((q = s.find_first_of(delims, p)) != string::npos) {
    if (q > p)
      vec.push_back(s.substr(p, q - p));
    p = q + 1;
  }
  if (p < s.length())
    vec.push_back(s.substr(p));
  return vec;
}

// this split function will protect string inside quote
vector<string> string_split_protect(const string& str, const string& delims) {
  vector<string> vec;
  string tmp = "";
  for (int i = 0; i < str.length(); i++) {
    if (is_white_space(str[i])) {
      vec.push_back(tmp);
      tmp = "";
    } else if (str[i] == '\"') {
      i++; // skip "
      while (str[i] != '\"' && i < str.length()) {
        tmp.push_back(str[i]);
        i++;
      }
      if (i == str.length())
        panic("unclosed quote");
    } else {
      tmp.push_back(str[i]);
    }
  }
  if (tmp.length() > 0)
    vec.push_back(tmp);
  return vec;
}

string string_split_last(const string& s, const string& delims) {
  int q;
  if ((q = s.find_last_of(delims)) != string::npos) {
    return s.substr(q + 1, s.length());
  }
  return s;
}

string string_split_first(const string& s, const string& delims) {
  int q;
  if ((q = s.find_first_of(delims)) != string::npos) {
    return s.substr(0, q);
  }
  return s;
}

string trim(const string& s) {
  if (s.length() == 0)
    return string(s);
  int p = 0, q = s.length() - 1;
  while (is_white_space(s[p]))
    p++;
  while (is_white_space(s[q]))
    q--;
  return s.substr(p, q - p + 1);
}