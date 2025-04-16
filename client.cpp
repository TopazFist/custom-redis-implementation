#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>


static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

// struct sockaddr_in {
//     uint16_t sin_family;
//     uint16_t sin_port;
//     struct in_addr sin_addr;
// }

// struct in_addr {
//     uint32_t s_addr;
// }

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if  (fd<0){
        die("socket()");
    }

    struct sockaddr_in addr = {};

    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));

    if (rv) {
        die("connect");
    }

    char msg[256];
    char rbuf[256];

    while (true){
        printf("Enter message (or 'quit'): ");

        if (!fgets(msg, sizeof(msg), stdin)) break;

        msg[strcspn(msg, "\n")] = 0;

        if (strcmp(msg, "quit") == 0 || strlen(msg) == 0) {
            printf("Exiting client.\n");
            break;
        }

        write(fd, msg, strlen(msg));

        ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
        if (n < 0) {
            die("read");
        }
        printf("server sent: %s\n", rbuf);
    }


    close(fd);
    return 0;

}