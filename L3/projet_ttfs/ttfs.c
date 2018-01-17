#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "ll.h"
#include "ttfs.h"

uint32_t TTTFS_VOLUME_BLOCK_COUNT;
uint32_t TTTFS_VOLUME_FREE_BLOCK_COUNT;
uint32_t TTTFS_VOLUME_FIRST_FREE_BLOCK;
uint32_t TTTFS_VOLUME_FREE_FILE_COUNT;
uint32_t TTTFS_VOLUME_FIRST_FREE_FILE;

time_t debut;


int tfs_format(int argc, char *argv[]){
	if( (argc == 5 || argc == 6) && (strcmp(argv[1],"-p") == 0) && (strcmp(argv[3],"-mf") == 0) ){
		char name[255];
		if( argc == 6 ){
			if(strcpy(name,argv[argc-1]) == NULL)
				return -1;
		}
		else {
			if(strcpy(name,"disk.tfs") == NULL)
				return -1;
		}
		
		uint32_t nump = (uint32_t)atoi(argv[2]);
		if( nump <= 0 )
			return -1;
		
		disk_id fd;	
		if( start_disk(name,&fd) == -1 )
			return -1;
			
		block b;
		if( read_block(fd,b,0) == -1 )
			return -1;
			
		if( nump > b[1] ){ // si le numero de la partition est superieur au nombre de partition
			stop_disk(fd);
			printf("Wrong number of partition\n");
			return -1;
		}
		
		int i;
		uint32_t sumtp = 1; // on le met a 1 a cause du block zero du disque
		for(i=2; i < nump; i++) // on fait la somme des tailles de chaque partition avant la partition dans laquelle on travaille
			sumtp += b[i];
		
		TTTFS_VOLUME_BLOCK_COUNT  = b[nump+1]; // b[nump+1] = la taille de la nump eme partition
		TTTFS_VOLUME_FREE_BLOCK_COUNT  = TTTFS_VOLUME_BLOCK_COUNT - 3;  
		//TTTFS_VOLUME_FIRST_FREE_BLOCK = ; // for i=2 i<nump i++ sum+=b[i] sum+=1; //le numero du premier bloc de la partition dans laquelle on travaille
		TTTFS_VOLUME_FREE_FILE_COUNT = 2;
		TTTFS_VOLUME_FIRST_FREE_FILE = 1;
		
		//int i;
		for(i=0; i < BLOCK_LENGTH; i++){
			b[i] = 0;
		}
		
		b[0] = TTTFS_MAGIC_NUMBER;
		b[1] = TTTFS_VOLUME_BLOCK_SIZE;
		b[2] = TTTFS_VOLUME_BLOCK_COUNT;
		b[3] = TTTFS_VOLUME_FREE_BLOCK_COUNT;
		b[4] = TTTFS_VOLUME_FIRST_FREE_BLOCK;
		b[5] = TTTFS_VOLUME_MAX_FILE_COUNT;
		b[6] = TTTFS_VOLUME_FREE_FILE_COUNT;
		b[7] = TTTFS_VOLUME_FIRST_FREE_FILE;
		
		
	}
	return -1;
}
