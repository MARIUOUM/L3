#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "ll.h"

/* _____________________________________________________ */
int main() {
    int argc;            // Nombre d'arguments.
    char argl[MAXLIN];   // Ligne de commande "brute".
    char* argv[MAXVEC];  // Tableau d'arguments.
    // On désactive le buffer de 'stdout'.
    // (Sinon il faudrait parfois utiliser 'fflush'.)
    setvbuf(stdout, NULL, _IONBF, 0);
    // Boucle infinie pouvant être interrompue par 'exit'.
    while (1) {
        // On affiche l'invite de commande.
        display_prompt();
        // On récupère la commande de l'utilisateur.
        read_command(argl);
        // On découpe cette commande en arguments.
        argc = tokenize_command(argl, argv);
        // On essaye d'exécuter la commande,
        // et on stocke la valeur de retour.
        execute_command(argc, argv);
    }
}

/* _____________________________________________________ */
// Affiche l'invite de commande sur 'stdout'.
void display_prompt() {
    printf(PROMPT);
}

/* _____________________________________________________ */
// Lit la prochaine ligne de commande depuis 'stdin'.
void read_command(char* argl) {
    if (fgets(argl, MAXLIN - 1, stdin) == NULL) {
        // Si l'utilisateur tape Ctrl+D,
        // on interprète ceci comme "exit".
        exit(0);
    } 
}

/* _____________________________________________________ */
// Découpe la ligne 'argl' au niveau des séparateurs ARGSEP
// et stocke un pointeur sur chaque argument dans 'argv'.
// (Les séparateurs dans 'argl' sont remplacés par '\0'.)
// Un '&' en fin de ligne est considéré comme un argument
// à part entière, même s'il est “collé” au précédent.
int tokenize_command(char* argl, char** argv) {
    int i;
    argv[0] = strtok(argl, ARGSEP);
    for (i = 0; argv[i] != NULL; ++i)
        argv[i+1] = strtok(NULL, ARGSEP);
    // Si un '&' est “collé” au dernier argument, on le
    // décale vers la droite, et on actualise 'argv'.
    // (On a retenu une place dans 'read_command'.)
    if (i > 0){
        char* last = argv[i-1];
        int l = strlen(last);
        if (l > 1 && last[l-1] == '&') {
            last[l-1] = '\0';
            last[l] = '&';
            last[l+1] = '\0';  // <-- La place retenue.
            argv[i] = last + l;
            argv[++i] = NULL;
        }
    }
    return i;
}

/* _____________________________________________________ */
// Essaye d'exécuter la commande spécifiée par 'argv',
// et renvoie son code de retour ou 1 en cas d'échec.
// Le travail est effectué par des fonctions auxiliaires.
// Cette fonction choisit seulement laquelle appeler.
int execute_command(int argc, char** argv) {
    if (strcmp(argv[0], "tfs_create") == 0)
        return tfs_create(argc, argv);
    if (strcmp(argv[0], "tfs_partition") == 0)
        return tfs_partition(argc, argv);
    if (strcmp(argv[0], "tfs_analyse") == 0)
        return tfs_analyse(argc, argv);
    else
        printf("Commande inconnue...\n");
        return 1;
}
