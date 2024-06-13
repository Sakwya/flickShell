#ifndef PANIC_H
#define PANIC_H

#include <STRING.h>

extern void panic(const std::string hint, bool exit_ = false, int exit_code = 0);

#endif // PANIC_H
