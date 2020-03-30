#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
/* -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb);
static void print_ino(const struct stat *sb);
static void print_perms(const struct stat *sb,char* pthname);
static void print_linkc(const struct stat *sb);
static void print_owner(const struct stat *sb);
static void print_size(const struct stat *sb);
static void print_laststch(const struct stat *sb);
static void print_lastacc(const struct stat *sb);
static void print_lastmod(const struct stat *sb);
static void print_name(const struct stat *sb, char *name);
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
    print_perms(&sb,argv[1]);
    print_linkc(&sb);
    print_owner(&sb);
    print_size(&sb);
    print_laststch(&sb);
    print_lastacc(&sb);
    print_lastmod(&sb);

    exit(EXIT_SUCCESS);
}
/* -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb){
  printf("File type:                ");
  switch (sb->st_mode & S_IFMT) {
  case S_IFMT:   printf("type of file\n");            break;
  case S_IFSOCK: printf("socket\n");                  break;
  case S_IFLNK:  printf("link\n");                    break;
  case S_IFREG:  printf("regular file\n");            break;
  case S_IFBLK:  printf("block device\n");            break;
  case S_IFDIR:  printf("directory\n");               break;
  case S_IFCHR:  printf("character device\n");        break;
  case S_IFIFO:  printf("FIFO\n");                    break;
  default:       printf("unknown situation\n");       break;
  }
}
/* -------------------------------------------------------------------------------- */

static void print_ino(const struct stat *sb){
    printf("I-node number:            %ld\n", (long) sb->st_ino);
}
/* -------------------------------------------------------------------------------- */
static void print_perms(const struct stat *sb,char* pthname){
    char* read = "no";
    char* write = "no";
    char* exec = "no";
    if(access(pthname, R_OK) == 0) read = "yes";
    if(access(pthname, W_OK) == 0) write = "yes";
    if(access(pthname, X_OK) == 0) exec = "yes";
  printf("Mode: %lo (octal)\n", (unsigned long) (sb->st_mode & ~S_IFMT));
  printf("Your permissions:         read: %s, write: %s, execute: %s\n",
  read,write,exec
  );
}
/* -------------------------------------------------------------------------------- */

static void print_linkc(const struct stat *sb){
    printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------------------- */

static void print_owner(const struct stat *sb){
    long uid = (long)sb->st_uid;
    long gid = (long)sb->st_gid;
	  struct passwd* u=getpwuid(uid);
	  struct group*  g=getgrgid(gid);
	  printf("Ownership:                %s(%ld)   %s(%ld)\n",u->pw_name, uid, g->gr_name, gid);
}
/* -------------------------------------------------------------------------------- */

static void print_size(const struct stat *sb){

    long long size = sb -> st_size;
    char* postfix = "bytes";

    if (size > 1048576){
        size = size / 1048576;
        postfix = "MB";
    }
    else if (size > 1024){
        size = size / 1024;
        postfix = "kB";
    }

    printf("Preferred I/O block size: %ld bytes\n", (long) sb->st_blksize);
    printf("File size:                %lld %s\n", size, postfix);
    printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */

static void print_laststch(const struct stat *sb){
	time_t now;
	time(&now);
	now-=sb->st_ctime;
	int nw,nd,nh,nm;
	for(nw=0;now>7*24*3600;nw+=1)
	now-=7*24*3600;
	for(nd=0;now>24*3600;nd+=1)
	now-=24*3600;
	for(nh=0;now>3600;nh+=1)
	now-=3600;
	for(nm=0;now>60;nm+=1)
	now-=60;
	printf("Last status change:       %d weeks %d days %d hours %d minutes ago\n", nw,nd,nh,nm);
}
/* -------------------------------------------------------------------------------- */

static void print_lastacc(const struct stat *sb){
	time_t now;
	time(&now);
	now-=sb->st_atime;
	int nw,nd,nh,nm;
	for(nw=0;now>7*24*3600;nw+=1)
	now-=7*24*3600;
	for(nd=0;now>24*3600;nd+=1)
	now-=24*3600;
	for(nh=0;now>3600;nh+=1)
	now-=3600;
	for(nm=0;now>60;nm+=1)
	now-=60;
	printf("Last file access:         %d weeks %d days %d hours %d minutes ago\n",nw,nd,nh,nm);
}
/* -------------------------------------------------------------------------------- */

static void print_lastmod(const struct stat *sb){
	time_t now;
	time(&now);
	now-=sb->st_mtime;
	int nw,nd,nh,nm;
	for(nw=0;now>7*24*3600;nw+=1)
	now-=7*24*3600;
	for(nd=0;now>24*3600;nd+=1)
	now-=24*3600;
	for(nh=0;now>3600;nh+=1)
	now-=3600;
	for(nm=0;now>60;nm+=1)
	now-=60;
	printf("Last file modification:   %d weeks %d days %d hours %d minutes ago\n",nw,nd,nh,nm);
}
/* -------------------------------------------------------------------------------- */

static void print_name(const struct stat *sb, char *name){
  struct stat sbt;
  lstat(name, &sbt);
  char* bname = basename(name);
    if ((sbt.st_mode & S_IFMT) == S_IFLNK){
	char buf[1024];
        char* file_name = realpath(name, buf);
        printf("Name of the file:         %s -> %s\n", bname, file_name);
    } else {
        printf("Name of the file:         %s\n", bname);
    }
}


/* -------------------------------------------------------------------------------- */

/*static void print_content(char *name){
    printf("\n\nWould you like to read %s content?[y/n]", name);
    char if_read;
    scanf("%c", &if_read);

    if(if_read == 'n'){
        return;
    }

    int fd;
    ssize_t bytes;
    char* buff[BFSIZE];
    fd = open(name, O_RDONLY);
    if (fd == -1){
        perror("File error!");
        exit(EXIT_FAILURE);
    }

    struct aiocb* cb;
    cb = malloc(sizeof(struct aiocb));

    cb -> aio_fildes = fd;
    cb -> aio_offset = 0;
    cb -> aio_buf = buff;
    cb -> aio_nbytes = BFSIZE;

    if (aio_read(cb) == -1){
        close(fd);
        exit(EXIT_FAILURE);
    }

    while(aio_error(cb) == EINPROGRESS){
        printf("%s", cb ->aio_buf);
    }


    close(fd);
}*/
