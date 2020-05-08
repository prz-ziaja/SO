#ifndef ELECTIONS_H
#define ELECTIONS_H
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#define MAX_VOTES 500
#define MAX_CLIENTS 2
#define MAX_COMMITTIES 2
#define MESSAGE_BUF_SIZE 100
#define SRV_KEYFILE_PATH_SIZE 1024
#define PERM_FILE S_IRUSR|S_IWUSR



struct message {
    long msgtype;
    long clid; 
    char data[MESSAGE_BUF_SIZE];
};



void make_srv_sv_queue_name(char* dest, size_t namemax) {
    snprintf(dest, namemax, "/tmp/srvsv_%s", getlogin());
}

int stdin_readall(char* dest, int MAXLEN) {
    char c;
    int bread = 0, b;
    do {
        b = scanf("%c", &c);
        dest[bread] = c;
        bread++;
    } while ((c != '\n') && (b == 1) && (bread < MAXLEN));

    dest[bread - 1] = '\0';

    return strcmp(dest, "q");
}
#endif