#include "am_shell.h"

int main() {
    printbanner();
    
    while (1) {
        char *line = am_read_line();
        if (!line)
            continue;

        char **args = am_split_line(line);

        if (args && args[0]) {
            if (!strcmp(args[0], "cd")) {
                if (args[1])
                    Chdir(args[1]);
                else
                    Chdir(getenv("HOME"));
            } else {
                am_execute(args);
            }
        }

        free(line);
        free(args);
    }

    return 0;
}
