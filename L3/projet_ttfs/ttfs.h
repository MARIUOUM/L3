#include <time.h>

#define TTTFS_MAGIC_NUMBER 0x31534654
#define TTTFS_VOLUME_BLOCK_SIZE 1024

#define TTTFS_VOLUME_MAX_FILE_COUNT 3

extern uint32_t TTTFS_VOLUME_BLOCK_COUNT;
extern uint32_t TTTFS_VOLUME_FREE_BLOCK_COUNT;
extern uint32_t TTTFS_VOLUME_FIRST_FREE_BLOCK;

extern uint32_t TTTFS_VOLUME_FREE_FILE_COUNT;
extern uint32_t TTTFS_VOLUME_FIRST_FREE_FILE;

#define TFS_REGULAR 0 
#define TFS_DIRECTORY 1
#define TFS_PSEUDO 2

#define TFS_DATE 0
#define TFS_DISK 1

extern time_t debut;


int tfs_format(int argc, char *argv[]);


int tfs_mkdir(const char *path, mode_t mode);
int tfs_rmdir(const char *path);
int tfs_rename(const char *old, const char *new);
int tfs_open(const char *name,int oflag, ...);
ssize_t tfs_read(int fildes,void *buf,size_t nbytes);
ssize_t tfs_write(int fildes,void *buf,size_t nbytes);
int tfs_close(int fildes);
off_t tfs_lseek(int fildes,off_t offset,int whence);
DIR *opendir(const char *filename);
struct dirent *readdir(DIR *dirp);
void rewinddir(DIR *dirp);
int closedir(DIR *dirp);

