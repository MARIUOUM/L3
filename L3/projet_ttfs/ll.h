#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*********************************************/
// macros de notre concole ttfs
#define MAXLIN 512           // Longueur max. d'une ligne.
#define MAXVEC (MAXLIN / 2)  // Nombre max. d'arguments.
#define ARGSEP " \t\n"       // Séparateurs d'arguments.
#define PROMPT "ttfs@confole$ "   // Invite de commande.

/* _____________________________________________________ */
// Fonctions principales.  (main.c)
// ---
void display_prompt();
void read_command(char* argl);
int tokenize_command(char* argl, char** argv);
int execute_command(int argc, char** argv);
/*
enum {
	ERROR_ID,
	ERROR_REPOSITION, 
	ERROR_READ_DISK,
	ERROR_WRITE_DISK,
	ERROR_READ_BLOC,
	ERROR_WRITE_BLOCK,
	ERROR_UNKNOWN, 
	ERROR_START, 
	ERROR_STOP
};
*/

#define MAX_BLOCK_SIZE 1024 // taille maximum d'un block

#define BLOCK_LENGTH 256 // 1024/4 // 4 representant 4 octets

typedef uint32_t block[BLOCK_LENGTH];

typedef int disk_id;

typedef int error;


/************************************************************/
// Fonctions de l'API de manipulation de disque

error start_disk(char *name, disk_id *id); //qui permet de manipuler un disque en lui associant une identite dynamique
error read_block(disk_id id, block b,uint32_t num); //qui permet de lire un bloc sur le disque
error write_block(disk_id id,block b,uint32_t num); //qui permet d’ecrire un bloc sur le disque 
error sync_disk(disk_id id); 
error stop_disk(disk_id id); //qui permet de terminer une session de travail sur un disque


/**********************************************************/
// Commandes de l'API
uint32_t little_endian_to_u32(uint32_t a);// convertit de  uint32 little endian en uint32 normal
uint32_t u32_to_little_endian(uint32_t a);
int tfs_create(int argc, char* argv[]); // cree un fichier qui contient les donnees du disque (le nombre de blocks)
int tfs_partition(int argc, char* argv[]); // partitionne l'espace physique du disque en paquets de blocks
int tfs_analyse(int argc, char* argv[]); // affiche les donnees du disk


