#ifndef AM_SHELL_H
#define AM_SHELL_H

#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sysexits.h>

// Couleurs ANSI
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define NC      "\033[0m"

#define ERROR(msg) fprintf(stderr, RED msg NC "\n")
#define DL      " \t\n\r\v\f" // délimiteurs

// Définition des codes de retour
enum {
    OK,
    ERROR
};

typedef struct s_builtin {
    const char *builtin_name;
    int (*foo)(char **av);
} t_builtin;

// Fonctions des commandes intégrées (à implémenter dans am_builtin.c)
int am_echo(char **args);
int am_env(char **args);
int am_exit(char **args);

// Fonctions d'animation et banner
void exitLoading(void);
void printbanner(void);

// Fonctions du shell (à implémenter dans am_shell.c)
char *am_read_line(void);
char **am_split_line(char *line);
void am_launch(char **args);
void am_execute(char **args);

// Wrappers avec gestion d'erreur (implémentés dans am_shell.c)
int Chdir(const char *path);
pid_t Fork(void);
void Execvp(const char *file, char *const argv[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *status, int options);
void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
char *Getcwd(char *buf, size_t size);
void Getline(char **lineptr, size_t *n, FILE *stream);

#endif
