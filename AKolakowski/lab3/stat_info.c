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
  
  if(sb->st_size/1024.0 < 1024)
		printf("File size:                %.2f KB\n", sb->st_size/1024.0);
  else
		printf("File size:                %.2f MB\n", sb->st_size/1024.0/1024.0);
	
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
	printf("You are:                  %s\n",you);
	printf("Owner is:                 %s\n",owner);
	printf("Your permissions:         read=%s write=%s execute=%s\n", read, write, execute);	
	
}
