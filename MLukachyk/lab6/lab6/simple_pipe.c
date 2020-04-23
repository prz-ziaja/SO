////
//// Created by Maryna Lukachyk on 2020-04-21.
////
//
//#include <stdio.h>
//#include <unistd.h>  //here we define pipe
//#include <string.h>
//#include <stdlib.h>
//
//int main(int argc, char *argv[]){
//
//    int pfd[2];
////    pipefd[1] is used for writing data to the pipe
////    pipefd[0] is used for reading data from the pipe
//    size_t nread;
//    char buf[100];
//    char * str ="Ur beautiful pipe example";
//    int pid;
//
//
//    pipe(pfd);
//    write(pfd[1], str, strlen(str));
//    nread=read(pfd[0],buf, sizeof(buf));
//
//
//    switch (pid = fork()) {
//        case -1:
//            exit(EXIT_FAILURE);
//        case 0 :
//            close(pfd[1]);
//            nread = read(pfd[0], buf, 100);
//            (nread != 0) ? printf("%s (%ld bytes)\n", buf, (long) nread) : printf("No data\n");
//            break;
//        default:
//            close(pfd[0]);
//            write(pfd[1], str, strlen(str));
//
//    }
//
//
//    return 0;
//}
//
//
