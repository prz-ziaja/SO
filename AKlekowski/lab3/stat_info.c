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
static void print_perms(const struct stat *sb);
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
        case S_IFBLK:  printf("urządzenie blokowe\n");      break;
        case S_IFCHR:  printf("urządzenie znakowe\n");      break;
        case S_IFDIR:  printf("katalog\n");                 break;
        case S_IFIFO:  printf("FIFO/pipe\n");               break;
        case S_IFLNK:  printf("dowiązanie symboliczne\n");  break;
        case S_IFREG:  printf("zwykły plik\n");             break;
        case S_IFSOCK: printf("gniazdo\n");                 break;
        default:       printf("nieznany typ\n");            break;
    }
}
/* -------------------------------------------------------------------------------- */

static void print_ino(const struct stat *sb){
    printf("I-node number:            %ld\n", (long) sb->st_ino);
}
/* -------------------------------------------------------------------------------- */
static void print_perms(const struct stat *sb){
    char* read = "no";
    char* write = "no";
    char* exec = "no";

    if (sb->st_mode & S_IRUSR || sb->st_mode & S_IRGRP || sb->st_mode & S_IROTH){
        read = "yes";
    }

    if (sb->st_mode & S_IWUSR || sb->st_mode & S_IWGRP || sb->st_mode & S_IWOTH){
        write = "yes";
    }

    if (sb->st_mode & S_IXUSR || sb->st_mode & S_IXGRP || sb->st_mode & S_IXOTH){
        exec = "yes";
    }

    printf("Your permissions: \t \t read: %s\t write: %s\t exec: %s\n", read, write, exec);
}
/* -------------------------------------------------------------------------------- */

static void print_linkc(const struct stat *sb){
    printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------------------- */

static void print_owner(const struct stat *sb){
    uid_t UID = sb->st_uid;
    gid_t GID =  sb->st_gid;

    struct passwd* user = getpwuid(UID);
    char* username = user -> pw_name;

    struct group* group = getgrgid(GID);
    char* groupname = group -> gr_name;

    printf("Ownership:                %s(%ld)   %s(%ld)\n",username , (long) UID, groupname, (long) GID);
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
    printf("Last status change:       %s", ctime(&sb->st_ctime));
}
/* -------------------------------------------------------------------------------- */

static void print_lastacc(const struct stat *sb){
    printf("Last file access:         %s", ctime(&sb->st_atime));
}
/* -------------------------------------------------------------------------------- */

static void print_lastmod(const struct stat *sb){
    printf("Last file modification:   %s", ctime(&sb->st_mtime));
}
/* -------------------------------------------------------------------------------- */

static void print_name(const struct stat *sb, char *name){
    char* bname = basename(name);
    if ((sb -> st_mode & S_IFMT) == S_IFLNK){
        unlink(name);
        char* file_name = NULL;
        symlink(file_name, name);
        printf("Name of the file:         %s -> %s\n", bname, file_name);
    } else {
        printf("Name of the file:         %s\n", bname);
    }
}
/* -------------------------------------------------------------------------------- */
