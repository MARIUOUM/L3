#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <endian.h>
#include "ll.h"
  

//============== Cree une un disque avec un le nombre de bloc donne ==========================
//
int tfs_create(int argc, char* argv[]){
	if((argc == 3 || argc == 4) && ( strcmp(argv[1],"-s") == 0 ) ){ // si on a 3 ou 4 arguments et que le 2eme argument est -s
		char name[255];
		if(argc == 3){ // sans presence de l'argument name
			if(strcpy(name,"disk") == NULL){ // on choisit disk par defaut
				perror("Error");
				return -1;
			}
		} 
		else { // si l'argument name est present
			if(strcpy(name,argv[3]) == NULL){
				perror("Error");
				return -1;
			}
		}
		
		int nb_blocks = atoi(argv[2]); // recuperation des nombres du size (nbre de blocks) passe en parametre
		if( nb_blocks <= 0 ){ // si size est infererieur ou egal a 0
			printf("Invalid size of disk.\n");
			return -1;
		}
		
		disk_id fd;
		if( (fd = open(strcat(name,".tfs"), O_WRONLY|O_CREAT, S_IRWXU)) == -1 ){ // si la creation & ouverture du fichier ne s'est pas bien passe
			perror("Error");
			return -1;
		}
		block b;
		int i;
		// On met le nombre de block dans la premiere case de notre block 0
		// par defaut on suppose que a la creation du disk on a une seule partition de taille nbre de block - 1 (1 representant le block 0)
		b[0] = (uint32_t)nb_blocks; // nombre total de block
		b[1] = (uint32_t)1; // nombre de partition
		b[2] = b[0] - b[1]; // taille de la partition
		for(i=3; i<BLOCK_LENGTH; i++){ // dans le reste du block on met que des 0
			b[i] = (uint32_t)0;
		}
		
		for(i=0; i<nb_blocks; i++){ // on ecrit nos n block dans le fichier ( disk)
			if( write_block(fd,b,i) == -1 ){
				return -1;
			}
			if(i==0){ 
				b[0] = (uint32_t)0;	
				b[1] = (uint32_t)0;
				b[2] = (uint32_t)0;
			
			}	
		}
		
			
		if( close(fd) == -1 ){ // fermeture du fichier (disk)
			return -1;
		}
	} else {
		printf("Invalid command: tfs_create -s size [name]\n");
	}
	return -1;
}

//======== partitionne un disque ne nombre de partion donne avec pour chacun sa taille =============
//
int tfs_partition(int argc, char* argv[]){
	if( argc>2 && ( strcmp(argv[1],"-p") == 0 ) ){ //si le nombre d'argument est superieur à 2 et le deuxième argument
		uint32_t np;
		char name[255];
		if( strcmp(argv[argc-2],"-p") == 0 ){// si l'avant dernier argument est -p ca veut dire que le name (du disk) n'est pas present
			if( (argc-1) % 2 != 0 )
				return -1;
			np = (argc-1)/2;
			if( strcpy(name,"disk.tfs") == NULL ){
				perror("");
				return -1;
			}
		}
		else { // avant dernier argument different de -p (le dernier argument c'est le name du disk)
			if( (argc-2) % 2 != 0 )
				return -1;
		 	np = (argc-2)/2;
		 	if( strcpy(name,argv[argc-1]) == NULL ){
		 		perror("");
				return -1;
			}
		}
		
		if( np == 1 ) // quand le nombre de partition est egal 1 on s'arrete automatiquement
			return 0;
		
		disk_id fd;
		if( start_disk(name,&fd) == -1 ){ // demarrage du disk
			perror(" ");
			return -1;
		}
		
		block b;
		if( read_block(fd,b,0) == -1 ) // lire le block zero
			return -1;

		int i;
		for(i=2; i<=b[1]; i++)
				b[i] = 0;
		
		
		int sum = 0;
		b[1] = np;
		int k = 2;
		for(i=2; i <= (np * 2); i+=2){ // On recupere la taille de chaque partition en testant a chaque fois si c'est positif
			int nbp = atoi(argv[i]);
			if( nbp <= 0 ){
				printf("Error negative or null partition\n");
				return -1;
			}
			b[k++] = ((uint32_t)nbp);
			sum += nbp;
		}
		
		if( sum > (b[0]-1) ){ // si la somme des tailles des partitions est superieur au nombre de block -1 on arrete le programme 
			printf("Invalid size of partitions\n");
			return -1;
		}
		
		if( write_block(fd,b,0) == -1 ) // on recrit le block zero avec les nouvelles informations des partitions
			return -1;
			
		if( stop_disk(fd) == -1 )
			return -1;
	} 
	else {
		printf("Invalid command: tfs_partition -p size [-p size]... [name]\n");
	}
	return 0;
}

//==== Analyse un disque en donnant son nombre de bloc et le nombre de patition et pour chaque partition sa taille ===========
//
int tfs_analyse(int argc, char *argv[]){
	if( argc == 1 || argc == 2 ){
		char name[255];
		if( argc == 2 ){ // sil ya 2 arguments cela veut dire que le nom du disk est present
			if( strcpy(name,argv[1]) == NULL ){
				perror("");
				return -1;
			}
		}
		else { // le nom du disk n'est pas present
			if( strcpy(name,"disk.tfs") == NULL ){
				perror("");
				return -1;
			}
		}
		disk_id fd;
		if( start_disk(name,&fd) == -1 ){
			perror("");
			return -1;
		}
		
		block b;
		if(read_block(fd,b,0) == -1) // lecture du block 0
			return -1;
		
		// on affiche toutes les informations du disk
		printf("Nombre de block: %d\n",b[0]);
		uint32_t n = b[1];
		if(n == 0){
			printf("Nombre de partition: %d\n",n);
		} 
		else {
			printf("Nombre de partition: %d\n",n);
			int i;
			for(i=2; i<=n+1; i++){
				printf("Partition %d taille: %d\n",i-1,b[i]);
			}
		}
		
		if( stop_disk(fd) == -1 )
			perror("");
			return -1;
	} 
	else {
		printf("Invalid command: tfs_analyze [name]\n");
	}
	return 0;
}
