#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "sv.h"

#define MAX_VOTES 500
#define MAX_CLIENTS 10
#define MAX_COMMITTIES 3

int main(void) {
    struct message msg;
    char svsrvname[SRV_KEYFILE_PATH_SIZE];
    ssize_t brcv,
            bsnd;
    int svsrvqid,
            svcntqid;
    key_t srvqkey;
    srand(time(0));
    int votes = rand() % MAX_VOTES / MAX_CLIENTS;
    int committee = rand() % MAX_COMMITTIES + 1;

    setbuf(stdout, NULL);

    /* Creating server key file name */
    make_srv_sv_queue_name(svsrvname, SRV_KEYFILE_PATH_SIZE);

    /* Getting server queue key */
    srvqkey = ftok(svsrvname, 1);
    if (srvqkey == -1) {
        printf("1FAIL!\nError: %s\n", strerror(errno));
        return 0;
    }

    /* Getting server queue identifier */
    svsrvqid = msgget(srvqkey, 0);
    if (svsrvqid == -1) {
        printf("2FAIL!\nError: %s\n", strerror(errno));
        return 0;
    }

    /* Getting client queue identifier */
    svcntqid = msgget(IPC_PRIVATE, PERM_FILE);
    if (svcntqid == -1) {
        printf("3FAIL!\nError: %s\n", strerror(errno));
        return 0;
    }

    /* Responce getting and sending */
    printf("Your message - committee %d \n", committee);
    snprintf(msg.data, MESSAGE_BUF_SIZE, "%d", votes);
    msg.clientid = svcntqid;
    msg.msgtype = committee;

    bsnd = msgsnd(svsrvqid, &msg, MESSAGE_BUF_SIZE, 0);
    if (bsnd == -1) {
        printf("4FAIL!\nError: %s\n", strerror(errno));
        msgctl(svsrvqid, IPC_RMID, NULL);
        msgctl(svcntqid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    while (1) {
        /* Reading from queue */
        brcv = msgrcv(svcntqid, &msg, MESSAGE_BUF_SIZE, 0, 0);
        if (brcv == -1) {
            printf("5FAIL!\nError: %s\n", strerror(errno));
            break;
        }
        printf("Message from server: %s\n", msg.data);
        if (strcmp("Finished", msg.data) == 0)
            break;
    }

    /* Cleaning up */
    msgctl(svsrvqid, IPC_RMID, NULL);
    msgctl(svcntqid, IPC_RMID, NULL);

    return 0;
}

