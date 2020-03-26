#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <aio.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
 

/* -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb);
static void print_ino(const struct stat *sb);
static void print_perms(const struct stat *sb, const char* path);
static void print_linkc(const struct stat *sb);
static void print_owner(const struct stat *sb);
static void print_size(const struct stat *sb);
static void print_laststch(const struct stat *sb);
static void print_lastacc(const struct stat *sb);
static void print_lastmod(const struct stat *sb);
static void print_name(const struct stat *sb, char *name);
static void print_content(char *name);
/* -------------------------------------------------------------------------------- */

int  main(int argc, char *argv[])
{
  struct stat sb;
  
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  if (stat(argv[1], &sb) == -1) {
    perror("stat");
    exit(EXIT_SUCCESS);
  }
  
  print_type(&sb);
  print_name(&sb, argv[1]);
  print_ino(&sb);
  print_perms(&sb, argv[1]);
  print_linkc(&sb);
  print_owner(&sb);
  print_size(&sb);
  print_laststch(&sb);
  print_lastacc(&sb);
  print_lastmod(&sb);
  print_content(argv[1]);  
  exit(EXIT_SUCCESS);
}
/* -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb){
  printf("File type:                ");
 switch (sb -> st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }
}
/* -------------------------------------------------------------------------------- */

static void print_ino(const struct stat *sb){
  printf("I-node number:            %ld\n", (long) sb->st_ino);
}
/* -------------------------------------------------------------------------------- */

static void print_perms(const struct stat *sb, const char* path){
  printf("Mode:                     %lo\n", (unsigned long) sb->st_mode  & (S_IRWXU | S_IRWXG | S_IRWXO));
   printf("Permissions:              read: %s, write: %s, execute: %s\n",
    access(path, R_OK) == 0 ? "yes" : "no",
    access(path, W_OK) == 0 ? "yes" : "no",
    access(path, X_OK) == 0 ? "yes" : "no"
  );
}
/* -------------------------------------------------------------------------------- */

static void print_linkc(const struct stat *sb){
  printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------------------- */

static void print_owner(const struct stat *sb){
  
  struct passwd *userName = getpwuid(sb->st_uid);
  struct group* group = getgrgid(sb->st_gid);
  printf("Ownership:                %s(%ld), %s(%ld)\n", userName -> pw_name, (long) sb->st_uid, group->gr_name, (long) sb->st_gid);
}
/* -------------------------------------------------------------------------------- */

static void print_size(const struct stat *sb){
  long long fileSize = sb -> st_size;
  char* nameFormat = "bytes";
  if(fileSize > 1048576){
   fileSize = fileSize/1048576;
   nameFormat = "MB";
}
  else if(fileSize > 1024) {
  fileSize = fileSize/1024;
  nameFormat = "kB";
}

  printf("Preferred I/O block size: %ld bytes\n", (long) sb->st_blksize);
  printf("File size:                %lld %s\n", fileSize, nameFormat);
  printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */

static void print_laststch(const struct stat *sb){
  time_t now;
  time(&now);
  now-=sb->st_ctime;
  char* timeFormat = "sec(s) ago";
  if (now > 86400) // 1 dzien - 86400 sekund
  {
    now = now/86400;
    timeFormat = "day(s) ago"; 
  }
  else if( now > 3600){
  now = now/3600;
  timeFormat = "hour(s) ago";  
}
  else if( now > 60)
{
  now = now/60;
  timeFormat = "minute(s) ago";
}
  printf("Last status change:       %d %s\n", now, timeFormat);
}
/* -------------------------------------------------------------------------------- */

static void print_lastacc(const struct stat *sb){
  time_t now;
  time(&now);
  now-=sb->st_atime;
  char* timeFormat = "sec(s) ago";
  if (now > 86400) // 1 dzien - 86400 sekund
  {
    now = now/86400;
    timeFormat = "day(s) ago"; 
  }
  else if( now > 3600){
  now = now/3600;
  timeFormat = "hour(s) ago";  
}
  else if( now > 60)
{
  now = now/60;
  timeFormat = "minute(s) ago";
}
  printf("Last file access:         %d %s\n",now, timeFormat);
}
/* -------------------------------------------------------------------------------- */

static void print_lastmod(const struct stat *sb){
time_t now;
  time(&now);
  now-=sb->st_mtime;
  char* timeFormat = "sec(s) ago";
  if (now > 86400) // 1 dzien - 86400 sekund
  {
    now = now/86400;
    timeFormat = "day(s) ago"; 
  }
  else if( now > 3600){
  now = now/3600;
  timeFormat = "hour(s) ago";  
}
  else if( now > 60)
{
  now = now/60;
  timeFormat = "minute(s) ago";
}
  printf("Last file modification:   %d %s\n", now, timeFormat);
}
/* -------------------------------------------------------------------------------- */

static void print_name(const struct stat *sb, char *name){
  struct stat sbt;
  lstat(name, &sbt);
  char* bname = basename(name);
    if ((sbt.st_mode & S_IFMT) == S_IFLNK){
	char buf[PATH_MAX];
        char* file_name = realpath(name, buf);
        printf("Name of the file:         %s -> %s\n", bname, file_name);
    } else {
        printf("Name of the file:         %s\n", bname);
    }
}
static void print_content(char *name){
    /*printf("\nAre u sure that u wanna read this  %s file[y/n]", name);
    char if_read;
    scanf("%c", &if_read);

    if(if_read == 'n'){
        exit(0);
    }

    int fd;
    ssize_t bytes;
    char* buff[512];

    fd = open(name, O_RDONLY);
    if (fd == -1){
        perror("Unable to open this file!");
        exit(EXIT_FAILURE);
    }

    struct aiocb* cb;
    cb = malloc(sizeof(struct aiocb));

    cb -> aio_fildes = fd;
    cb -> aio_offset = 0;
    cb -> aio_buf = buff;
    cb -> aio_nbytes = sizeof(buff);

    if (aio_read(cb) == -1){
        close(fd);
        exit(EXIT_FAILURE);
    }

    while(aio_error(cb) == EINPROGRESS){
        printf("%s", cb ->aio_buf);
    }


    close(fd);*/
}
/* -------------------------------------------------------------------------------- */
