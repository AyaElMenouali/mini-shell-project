#include "am_shell.h"

// === Tableau des commandes intégrées ===
t_builtin g_builtin[] = {
    {"echo", am_echo},
    {"env", am_env},
    {"exit", am_exit},
    {NULL, NULL}
};

// === Implémentation des commandes intégrées ===

int am_echo(char **args) {
    int start = 1;
    bool newline = true;

    if (!args || !args[0])
        return 1;

    if (args[1] && !strcmp(args[1], "-n")) {
        newline = false;
        start = 2;
    }

    for (int i = start; args[i]; i++) {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
    }

    if (newline)
        printf("\n");

    return 0;
}

int am_env(char **args) {
    extern char **environ;
    (void)args;

    if (!environ)
        return 1;

    for (int i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);

    return 0;
}

int am_exit(char **args) {
    (void)args;

    exitLoading();
    exit(EX_OK);
}

// === Fonctions d'animation ===

void exitLoading() {
    const char *spinner = "|/-\\";
    printf(RED "Arrêt en cours " NC);

    for (int i = 0; i < 40; i++) {
        printf("\r" RED "Arrêt en cours %c" NC, spinner[i % 4]);
        fflush(stdout);
        usleep(100000);
    }

    printf("\r" CYAN "✅ Arrêt terminé !     \n" NC);
}

 

void printbanner() {
    // Efface l'écran (optionnel)
    printf("\033[2J\033[H");

    // Art ASCII avec dégradé de couleurs
    printf(RED "     _______  __   __    _______  __   __  _______  ___      ___       \n");
    printf(RED"     |   _   ||  |_|  |  |       ||  | |  ||       ||   |    |   |      \n");
    printf(RED"     |  |_|  ||       |  |  _____||  |_|  ||    ___||   |    |   |      \n");
    printf(RED"     |       ||       |  | |_____ |       ||   |___ |   |    |   |      \n");
    printf(RED"     |       ||       |  |_____  ||       ||    ___||   |___ |   |___   \n");
    printf(RED"     |   _   || ||_|| |   _____| ||   _   ||   |___ |       ||       |  \n" );
    printf(RED"     |__| |__||_|   |_|  |_______||__| |__||_______||_______||_______|  \n" );

    
    // Ligne de séparation
    printf(CYAN "\n          ════════════════════════════════════════\n");
    
    // Message de bienvenue centré
    printf(BLUE "          🚀 BIENVENUE DANS AM SHELL 🚀\n");
    
    // Informations supplémentaires
    printf(CYAN "          ════════════════════════════════════════\n" NC);
    printf("           Développeur : Aya Elmenouali\n");
    printf("           Version     : 1.0 (Stable)\n");
    printf("           Date        : %s\n", __DATE__);
    printf("           Heure       : %s\n", __TIME__);
    printf(CYAN "          ════════════════════════════════════════\n\n" NC);
    
    // Petit message d'aide
    printf("Tapez 'help' pour voir les commandes disponibles\n\n");
}
