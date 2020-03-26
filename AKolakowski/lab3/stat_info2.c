#define _BSD_SOURCE

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
#include <time.h>
#include <string.h>
#include <aio.h>
#include <errno.h>
/* -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb);
static void print_ino(const struct stat *sb);
static void print_perms(const struct stat *sb);
static void print_linkc(const struct stat *sb);
static void print_owner(const struct stat *sb);
static void print_size(const struct stat *sb);
static void print_laststch(const struct stat *sb);
static void print_lastacc(const struct stat *sb);
static void print_lastmod(const struct stat *sb);
static void print_name(const struct stat *sb, char *name);
static void print_my(const struct stat *sb);
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
  print_perms(&sb);
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
  switch (sb->st_mode & S_IFMT) {
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

static void print_perms(const struct stat *sb){
  printf("Mode:                     %lo (octal)\n", (unsigned long) (sb->st_mode & ~S_IFMT));
  
  print_my(sb);
}
/* -------------------------------------------------------------------------------- */

static void print_linkc(const struct stat *sb){
  printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------------------- */
static void print_owner(const struct stat *sb){
  printf("Ownership:                %s(%ld)   %s(%ld)\n", (getpwuid(sb->st_uid))->pw_name, (long) sb->st_uid, (getgrgid(sb->st_gid))->gr_name, (long) sb->st_gid);
}
/* -------------------------------------------------------------------------------- */

static void print_size(const struct stat *sb){
  printf("Preferred I/O block size: %ld bytes\n", (long) sb->st_blksize);
  printf("File size:                %.2f MB\n", sb->st_size/1024.0);
  printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */

static void print_laststch(const struct stat *sb){
  printf("Last status change:       %d days ago\n", (int)(difftime(time(NULL),sb->st_ctime)/86400));
}

/* -------------------------------------------------------------------------------- */

static void print_lastacc(const struct stat *sb){
  printf("Last file access:         %d days ago\n", (int)(difftime(time(NULL),sb->st_atime)/86400));
}
/* -------------------------------------------------------------------------------- */

static void print_lastmod(const struct stat *sb){
  printf("Last file modification:   %d days ago\n", (int)(difftime(time(NULL),sb->st_mtime)/86400));
}
/* -------------------------------------------------------------------------------- */

static void print_name(const struct stat *sb, char *name){
  char* bname = basename(name);

  char buf[256];
  int r;

  r=readlink(bname,buf,256);

  if(r==-1)
	printf("Name of the file:         %s\n", bname);
  else
	printf("Name of the file:         %s -> %s\n", bname, buf);
  
}
/* -------------------------------------------------------------------------------- */

static void print_my(const struct stat *sb)
{
	char *read, *write, *execute;

	char *you = getlogin();
	char *owner = getpwuid(sb->st_uid)->pw_name;

	if(strcmp(you,owner)==0)
	{
		if(sb->st_mode & S_IRUSR)/* owner has read permission */
			read="true";
		else
			read="false";

		if(sb->st_mode & S_IWUSR)/* owner has write permission */
			write="true";
		else
			write="false";

		if(sb->st_mode & S_IXUSR)/* owner has execute permission */
			execute="true";
		else
			execute="false";
	}
	else
	{
		if(sb->st_mode & S_IROTH)/* others have read permission */
			read="true";
		else
			read="false";

		if(sb->st_mode & S_IWOTH)/* others have write permission */
			write="true";
		else
			write="false";

		if(sb->st_mode & S_IXOTH)/* others have execute permission */
			execute="true";
		else
			execute="false";	
	}
	printf("You are: %s, owner is: %s\n",you,owner);
	printf("Your permissions: read=%s write=%s execute=%s\n", read, write, execute);	
	
}

static void print_content(char *name)
{
	char c;
	printf("\nCzy chcesz wypisac zawartosc pliku %s? (t/n)\n",name);
	c = getchar();

	if(c=='t')
	{
		int BUFSIZE=256;
		char buf[BUFSIZE];

		int fd;		
		int nbytes;

		int rd, sp;
		struct aiocb cb;
		const struct aiocb *list[1] = { &cb };
		memset(&cb, 0, sizeof(cb));
		
		fd = open(name, O_RDONLY);
		if(fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}

		cb.aio_fildes = fd;
		cb.aio_buf = buf;
		cb.aio_nbytes = sizeof(buf);

		while (1)
		{
			rd=aio_read(&cb);
			if(rd == -1)
			{
				perror("aio_read");
				exit(EXIT_FAILURE);
			}
			
			sp=aio_suspend(list,1,NULL);
			if(sp == -1)
			{
				perror("aio_suspend");
				exit(EXIT_FAILURE);
			}

			nbytes=aio_return(&cb);
			if(nbytes == -1)
			{
				perror("aio_return");
				exit(EXIT_FAILURE);
			}

			if(nbytes == 0)
				break;

			write(1,buf,nbytes);	

			cb.aio_offset += nbytes;
		}	
	}	
}