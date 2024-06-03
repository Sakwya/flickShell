#include<shell.h>

using namespace std;


int main() {
    rl_initialize();
    using_history();
    fflush(stdout);
    rl_on_new_line();
    rl_bind_key('\t', rl_complete);

    init_shell();

    reader_loop();
    return 0;
}
