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

int isIn(long * array, long element, int amount) {
    int i = 0;
    for (i = 0; i < amount; i++) {
        if (element == array[i])
            return 1;
    }
    return 0;
}

void printResults(int * votesInCommitties, int sumOfVotes, int amount) {
    int i = 0, max = 0, index = -1;
    printf("Attendance: %f %% \n", (float) (sumOfVotes * 100) / (amount * MAX_VOTES / MAX_CLIENTS));
    for (i = 0; i < MAX_COMMITTIES; i++) {
        if (votesInCommitties[i] > max) {
            max = votesInCommitties[i];
            index = i;
        }
    }
    printf("Actual winner: %d \n", index);
}

int main(void) {
    struct message msg;
    char svsrvname[SRV_KEYFILE_PATH_SIZE];
    long clientsIDs[MAX_CLIENTS];
    ssize_t brcv,
            bsnd;
    int kfor, /* Key File Opening Result */
            svsrvqid, amountOfClients = 0, i;
    key_t qkey;
    int votesSum = 0;
    int results[MAX_COMMITTIES];

    for (i = 0; i < MAX_COMMITTIES; i++) {
        results[i] = 0;
    }
    setbuf(stdout, NULL);
    msg.clientid = -1;

    /* Creating server key file */
    make_srv_sv_queue_name(svsrvname, SRV_KEYFILE_PATH_SIZE);
    kfor = open(svsrvname, O_WRONLY | O_CREAT, PERM_FILE);
    if (kfor == -1) {
        printf("1FAIL!\nError: %s\n", svsrvname, strerror(errno));
        return 0;
    }
    close(kfor);

    /* Getting queue key */
    qkey = ftok(svsrvname, 1);
    if (qkey == -1) {
        printf("2FAIL!\nError: %s\n", strerror(errno));
        return 0;
    }

    /* Getting server queue identifier */
    svsrvqid = msgget(qkey, PERM_FILE);
    if (svsrvqid == -1) {
        msgctl(svsrvqid, IPC_RMID, NULL);
        svsrvqid = msgget(qkey, IPC_CREAT | PERM_FILE);
    }
    if (svsrvqid == -1) {
        printf("3FAIL!\nError: %s\n", strerror(errno));
        return 0;
    }


    while (1) {
        /* Reading from queue */
        brcv = msgrcv(svsrvqid, &msg, MESSAGE_BUF_SIZE, 0, 0);
        if (brcv == -1)
            break;
        votesSum += atoi(msg.data);
        results[msg.msgtype - 1] += atoi(msg.data);

        printf("Message from comittee [%ld]: %s\n", msg.clientid, msg.data);
        if (!isIn(clientsIDs, msg.clientid, amountOfClients)) {
            clientsIDs[amountOfClients++] = msg.clientid;
        }

        /* Responce getting and sending */
        snprintf(msg.data, MESSAGE_BUF_SIZE, "Sum of votes: %d \n", votesSum);

        for (i = 0; i < amountOfClients; i++) {
            bsnd = msgsnd(clientsIDs[i], &msg, MESSAGE_BUF_SIZE, 0);
            if (bsnd == -1) {
                printf("4FAIL!\nError: %s\n", strerror(errno));
                break;
            }
        }
        printResults(results, votesSum, amountOfClients);
        if (amountOfClients == MAX_COMMITTIES) {
            snprintf(msg.data, MESSAGE_BUF_SIZE, "%s", "Finished");
            for (i = 0; i < amountOfClients; i++) {
                bsnd = msgsnd(clientsIDs[i], &msg, MESSAGE_BUF_SIZE, 0);
                if (bsnd == -1) {
                    printf("5FAIL!\nError: %s\n", strerror(errno));
                    break;
                }
            }
            break;
        }
    }

    /* Cleaning up */
    unlink(svsrvname);
    msgctl(svsrvqid, IPC_RMID, NULL);

    return 0;
}

