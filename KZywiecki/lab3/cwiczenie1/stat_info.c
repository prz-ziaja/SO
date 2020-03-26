#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <aio.h>
#include <unistd.h>
#include <fcntl.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
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
static void print_content(char* name);
/* -------------------------------------------------------------------------------- */

int getMonth(char* name)
{
    if(strcmp(name, "JAN") == 0) return 1;
    if(strcmp(name, "FEB") == 0) return 2;
    if(strcmp(name, "MAR") == 0) return 3;
    if(strcmp(name, "APR") == 0) return 4;
    if(strcmp(name, "MAY") == 0) return 5;
    if(strcmp(name, "JUN") == 0) return 6;
    if(strcmp(name, "JUL") == 0) return 7;
    if(strcmp(name, "AUG") == 0) return 8;
    if(strcmp(name, "SEP") == 0) return 9;
    if(strcmp(name, "OCT") == 0) return 10;
    if(strcmp(name, "NOV") == 0) return 11;
    if(strcmp(name, "DEC") == 0) return 12;
}

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
    printf("Permissions:              ");
    mode_t val=(sb->st_mode & ~S_IFMT);
    int mode = 0;
    if (val & S_IRUSR) mode += 4;
    if (val & S_IWUSR) mode += 2;
    if (val & S_IXUSR) mode += 1;
    printf("%d", mode);
    mode = 0;
    if (val & S_IRGRP) mode += 4;
    if (val & S_IWGRP) mode += 2;
    if (val & S_IXGRP) mode += 1;
    printf("%d", mode);
    mode = 0;
    if (val & S_IROTH) mode += 4;
    if (val & S_IWOTH) mode += 2;
    if (val & S_IXOTH) mode += 1;
    printf("%d\n", mode);

    printf("Your permissions: read: %s, write: %s, execute: %s\n",
    val&S_IRUSR ? "Yes" : "No",
    val&S_IWUSR ? "Yes" : "No",
    val&S_IXUSR ? "Yes" : "No");
}
/* -------------------------------------------------------------------------------- */
    
static void print_linkc(const struct stat *sb){
    printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------------------- */

static void print_owner(const struct stat *sb){
    struct passwd *pw = getpwuid(sb->st_uid);
    struct group *gr = getgrgid(sb->st_gid);
    printf("Ownership: %s(%ld), %s(%ld)\n", pw->pw_name,
    sb->st_uid, gr->gr_name, sb->st_gid);
}
/* -------------------------------------------------------------------------------- */
    
static void print_size(const struct stat *sb){
    long long size = sb -> st_size;
    int n = 0;
    while(size > 1024)
    {
        n++;
        size /= 1024;
    }
    printf("Preferred I/O block size: %ld bytes\n", (long) sb->st_blksize);
    char suffix[6];
    switch(n)
    {
        case 0:
            memcpy(suffix, "B", 2); break;
        case 1:
            memcpy(suffix, "kB", 3); break;
        case 2:
            memcpy(suffix, "MB", 3); break;
        case 3:
            memcpy(suffix, "GB", 3); break;
        case 4:
            memcpy(suffix, "TB", 3); break;
        default:
            memcpy(suffix, "u ok?", 6); break;
    }
    printf("File size:                %lld %s\n", size, suffix);
    printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */
    
static void print_laststch(const struct stat *sb){
    /*
    Zadanie 1.9 - niewykonane
    char month[10];
    unsigned int day, hour, year;
    int i = sscanf(ctime(&sb->st_ctime), "%*s %s %d %d:%*d:%*d %d", 
    month,  day, hour, year);
    if(i == -1)
    {
        printf(strerror(errno));
    }
    */
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
    //1.10 tez
    struct stat status;
    lstat(name, &status);
    char buf[512];
    if(status.st_size == (sb->st_size))
    {
        printf("aaa");
        readlink(name, buf, 511);
        printf(buf);
    }
    else
    {
        char* bname = basename(name);
        printf("Name of the file:         %s\n", bname);
    }
    
}

void print_content(char* name)
{
    printf("Czy chcesz wypisac zawartosc pliku? (y/n)");
    char ch;
    scanf("%c", &ch);
    if(ch == 'Y' || ch == 'y')
    {
        int fd = open(name, O_RDONLY);
        struct aiocb data;
        memset(&data, 0, sizeof(struct aiocb));

        char buf[512];
        int n;
        int ret;

        data.aio_nbytes = 511;
        data.aio_buf = buf;
        data.aio_fildes = fd;
        data.aio_lio_opcode = LIO_WRITE;

        do
        {

            n = aio_read(&data);
            if(n == -1)
            {
                printf(strerror(errno));
                break;
            }
            int error;
            while(error = aio_error(&data) == EINPROGRESS)
            {
                
            }
            ret = aio_return(&data);
            if(ret != 511)
            {
                printf("%d", ret);
            }
            printf(buf);
        } while (ret == 511);
        close(fd);
    }
}

/* -------------------------------------------------------------------------------- */

