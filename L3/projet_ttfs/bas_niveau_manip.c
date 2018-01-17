#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <endian.h>
#include "ll.h"


//============= Tranforme un uint32 little endian en uint32 normal ===========================
// 
uint32_t little_endian_to_u32(uint32_t a)
{
  uint8_t l[4];
  
  *(uint32_t *) l = a;
 
  return 256*256*256*l[3] + 256*256*l[2] + 256*l[1] + l[0];
}

uint32_t u32_to_little_endian(uint32_t a)
{
  uint8_t l [4];

    l[3] = (((a/256)/256)/256) % 256;
    l[2] = ((a/256)/256) % 256;
    l[1] = (a/256) % 256;
    l[0] = a % 256;

    return *(uint32_t *) l;
}

error read_physical_block(disk_id id,block b,uint32_t num){
	if( id != -1){
		if( lseek(id,MAX_BLOCK_SIZE*num,SEEK_SET) == -1 ){ // deplacement dans le num eme block du fichier (disk)
			return -1;
		}
		uint32_t *buf = malloc(sizeof(uint32_t));
		int i;
		for(i=0; i < BLOCK_LENGTH; i++){ // on lit tout le block et on stocke les informations lues dans notre block b
			if( read(id,buf,sizeof(uint32_t)) == -1 ){
				return -1;
			}
			b[i] = little_endian_to_u32(*buf);
		}
		return 0;
	}
	return -1;
}

error write_physical_block(disk_id id,block b,uint32_t num){
	if( id != -1){
		if( lseek(id,MAX_BLOCK_SIZE*num,SEEK_SET) == -1 ){// deplacement dans le num eme block du fichier (disk)
			return -1;
		}
		int i;
		for(i=0; i < BLOCK_LENGTH; i++){ //on ecrit toutes les informations de notre block b dans le fichier (disk)
			uint32_t buf = u32_to_little_endian(b[i]);
			if( write(id,&buf,sizeof(uint32_t)) == -1 ){
				return -1;
			}
		}
		return 0;
	}
	return -1;
}

/************** Fontions de manipulation visibles par les couches superieurs **************************/
// start_disk manipule un disk en lui associant une identite dynamique
// 
error start_disk(char *name,disk_id *id){
	if( ( *id = open(name,O_RDWR,S_IRWXU) ) == -1 ){
		perror("Error");
		return -1;
	}
	return 0;
}


error read_block(disk_id id,block b,uint32_t num){
	return read_physical_block(id,b,num);
}

error write_block(disk_id id,block b,uint32_t num){
	return write_physical_block(id,b,num);
}

error sync_disk(disk_id id){
	return 0;
}

// fermeture du disk (du fichier)
error stop_disk(disk_id id){
	return close(id);
}

