#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void tail(char *path) {
	char c;
	int n_line = 0;
	int fd = open(path, O_RDONLY);
    off_t tail, end;
	tail = end = lseek(fd, 1, SEEK_END);

    while (n_line <= 10 && tail > 0) {
        if (pread(fd, &c, 1, tail - 1) == -1) {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }

        if (c == '\n') {
            n_line++;
            if (n_line == 11) {
                tail++;
            }
        }
        tail--;
    }

    for(int i=tail; i<end-1; i++) {
        if (pread(fd, &c, 1, i) == -1) {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }

        printf("%c", c);
    }

    if (close(fd) == -1) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]) {
    tail(argv[1]);
}
