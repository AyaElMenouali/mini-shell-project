#include "am_shell.h"

// === Variables globales ===
int status = 0;

// === Déclarations externes ===
extern t_builtin g_builtin[];

// === Wrappers système avec gestion d'erreurs ===

int Chdir(const char *path) {
    if (!path) {
        ERROR("cd: missing path argument");
        return -1;
    }
    if (chdir(path) == -1) {
        perror(RED "cd failed" NC);
        return -1;
    }
    return 0;
}

pid_t Fork(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror(RED "fork failed" NC);
        exit(EXIT_FAILURE);
    }
    return pid;
}

void Execvp(const char *file, char *const argv[]) {
    if (!file || !argv) {
        fprintf(stderr, RED "Execvp: invalid arguments\n" NC);
        exit(EXIT_FAILURE);
    }

    if (execvp(file, argv) == -1) {
        perror(RED "command failed" NC);
        exit(EX_UNAVAILABLE);
    }
}

pid_t Wait(int *status) {
    if (!status) {
        fprintf(stderr, RED "Wait: status required\n" NC);
        return -1;
    }

    pid_t result = wait(status);
    if (result == -1)
        perror(RED "Wait failed" NC);

    if (WIFEXITED(*status))
        *status = WEXITSTATUS(*status);

    return result;
}

void *Malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr && size != 0) {
        perror(RED "malloc failed" NC);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *Realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr && size != 0) {
        perror(RED "realloc failed" NC);
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

char *Getcwd(char *buf, size_t size) {
    char *result = getcwd(buf, size);
    if (!result)
        perror(RED "getcwd failed" NC);
    return result;
}

void Getline(char **lineptr, size_t *n, FILE *stream) {
    if (!lineptr || !stream) {
        fprintf(stderr, RED "getline: invalid arguments\n" NC);
        return;
    }

    if (getline(lineptr, n, stream) == -1) {
        free(*lineptr);
        *lineptr = NULL;
        if (feof(stream))
            printf(RED "[EOF]" NC "\n");
        else
            perror(RED "getline failed" NC);
    }
}

// === Fonctions utilitaires ===

char *get_username() {
    struct passwd *pw = getpwuid(getuid());
    return pw ? pw->pw_name : "unknown";
}

int is_root() {
    return getuid() == 0;
}

// === Fonctions du shell ===

char *am_read_line(void) {
    char *line = NULL;
    size_t bufsize = 0;
    char cwd[BUFSIZ];

    if (!getcwd(cwd, sizeof(cwd)))
        strcpy(cwd, "?");

    char *user = get_username();

    if (is_root()) {
        printf(RED "[%s" NC "👿" BLUE "%s" RED "] ># " NC, user, cwd);
    } else {
        printf(GREEN "[%s" NC "😃" CYAN "%s" GREEN "] >$ " NC, user, cwd);
    }

    if (getline(&line, &bufsize, stdin) == -1) {
        perror("Erreur de lecture");
        free(line);
        return NULL;
    }

    return line;
}

char **am_split_line(char *line) {
    size_t bufsize = BUFSIZ;
    size_t position = 0;
    char **tokens = Malloc(bufsize * sizeof(*tokens));
    char *token;

    for (token = strtok(line, DL); token; token = strtok(NULL, DL)) {
        tokens[position++] = token;
        if (position >= bufsize) {
            bufsize *= 2;
            tokens = Realloc(tokens, bufsize * sizeof(*tokens));
        }
    }

    tokens[position] = NULL;
    return tokens;
}

void am_launch(char **args) {
    pid_t pid = Fork();

    if (pid == 0) {
        // Processus enfant
        Execvp(args[0], args);
    } else {
        // Processus parent
        Wait(&status);
    }
}

void am_execute(char **args) {
    int i = 0;
    const char *curr_builtin;

    while ((curr_builtin = g_builtin[i].builtin_name)) {
        if (!strcmp(args[0], curr_builtin)) {
            status = g_builtin[i].foo(args);
            return;
        }
        i++;
    }

    am_launch(args);
}
