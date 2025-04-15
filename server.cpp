#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>   
#include <sys/socket.h>
// #include <netinet/ip.h>

static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

// struct in_addr {
//     uint32_t s_addr;
// };

// struct sockaddr_in {
//     uint16_t sin_family;
//     uint16_t sin_port;
//     struct in_addr sin_addr;
// };



static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        msg("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}


int main() {
    //initializes socket
    //AF_INET address family (IPv4)
    //specifies (tcp)
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        die("socket()");
    }

    //sets SO_REUSEADDR
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    
    //creates socket
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);        // port
    addr.sin_addr.s_addr = htonl(0); 

    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) { die("bind()"); }

    //create listener (this is where the socket is actually created)
    rv = listen(fd, SOMAXCONN);
    if (rv) { die("listen()"); }

    while (true){
        //accept
        struct sockaddr_in client_addr = {};

        socklen_t addrlen = sizeof(client_addr);

        //The accept() syscall also returns the peer’s address. The addrlen argument is both the input and output size.
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);

        if (connfd < 0) {
            continue;   // error
        }
        do_something(connfd);
        close(connfd);

    }

    return 0;
}