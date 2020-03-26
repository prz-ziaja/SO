#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <aio.h>
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
static void print_content(char* name);
/* -------------------------------------------------------------------------------- */

int  main(int argc, char *argv[])
{
    struct stat sb;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (lstat(argv[1], &sb) == -1) {
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
    char* r="no";
    char* w="no";
    char* x="no";
    if (sb->st_mode & S_IRUSR || sb->st_mode & S_IRGRP || sb->st_mode & S_IROTH){
        r = "yes";
    }

    if (sb->st_mode & S_IWUSR || sb->st_mode & S_IWGRP || sb->st_mode & S_IWOTH){
        w = "yes";
    }

    if (sb->st_mode & S_IXUSR || sb->st_mode & S_IXGRP || sb->st_mode & S_IXOTH){
        x = "yes";
    }
    printf("Your permisions: read: %s, write: %s, execute: %s\n", r, w, x);
}
/* -------------------------------------------------------------------------------- */

static void print_linkc(const struct stat *sb){
    printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------------------- */

static void print_owner(const struct stat *sb){
    struct passwd* usr_pswd = getpwuid(sb->st_uid);
    char* username = usr_pswd->pw_name;
    struct group* group = getgrgid(sb->st_gid);
    char* groupname = group -> gr_name;

    printf("Ownership:                User %s(%ld)   Group %s(%ld)\n", username,(long) sb->st_uid, groupname, (long) sb->st_gid);
}
/* -------------------------------------------------------------------------------- */
static void print_size(const struct stat *sb){
    long long size = sb->st_size;
    char *size_unit="bytes";
    if (size > 1048576){
        size = size / 1048576;
        size_unit = "MB";
    }
    else if (size > 1024){
        size = size / 1024;
        size_unit = "kB";
    }
    printf("Preferred I/O block size: %ld bytes\n", (long) sb->st_blksize);
    printf("File size:                %lld %s\n",size, size_unit);
    printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */
void calculate_time(char *action, time_t last_time){
    time_t current;
    current = time(NULL);
    double seconds =difftime(current, last_time);
    int days = (int) seconds / 86400;
    double hours_d = seconds - days*86400;
    int hours = (int) floor(hours_d/3600);
    double min_d = hours_d - hours*3600;
    int min = (int) floor(min_d/60);
    int sec = (int) floor(min_d - min*60);
    printf("Last %s:       %s - %d day(s) %d hour(s) %d minute(s) %d second(s) ago\n", action, ctime(&last_time), days, hours, min, sec);
}

static void print_laststch(const struct stat *sb){
    time_t chng = sb->st_ctime;
    char *action="status change";
    calculate_time(action, sb->st_ctime);
}
/* -------------------------------------------------------------------------------- */

static void print_lastacc(const struct stat *sb){
    time_t acc = sb->st_atime;
    char *action="file access";
    calculate_time(action, sb->st_atime);
}
/* -------------------------------------------------------------------------------- */

static void print_lastmod(const struct stat *sb){
    time_t modified = sb->st_mtime;
    char *action="file modification";
    calculate_time(action, modified);
}
/* -------------------------------------------------------------------------------- */

static void print_name(const struct stat *sb, char *name){
    char* bname = basename(name);
    if ((sb -> st_mode & S_IFMT) == S_IFLNK){
        char *buf;
        readlink(bname, buf, 100);
        printf("Name of the file:         %s -> %s\n", bname, buf);
    } else {
        printf("Name of the file:         %s\n", bname);
    }

}
/* ---------------------------------------------------------------------------------- */
static void print_content(char *name){
    printf("\nDo you want to get content of file %s?[y/n]", name);
    char answer;
    scanf("%c", &answer);

    if( answer == 'n'){
        return;
    }

    int fd;
    ssize_t bytes;
    char* buff[256];

    fd = open(name, O_RDONLY);
    if (fd == -1) {
        printf("Cannot open file \n");
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


    close(fd);
}

