
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define NAME_LEN 32

int sockfd_global = -1;

void *recv_handler(void *arg) {
    int sockfd = *(int*)arg;
    char buff[BUF_SIZE];
    while (1) {
        ssize_t len = recv(sockfd, buff, sizeof(buff)-1, 0);
        if (len <= 0) {
            printf("\nDisconnected from server.\n");
            close(sockfd);
            exit(0);
        }
        buff[len] = '\0';
        // Print incoming message
        printf("%s", buff);
        // reprint prompt if needed
        fflush(stdout);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <server-ip> <server-port> <username>\n", argv[0]);
        return 1;
    }
    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    char username[NAME_LEN];
    strncpy(username, argv[3], NAME_LEN-1);
    username[NAME_LEN-1] = '\0';

    int sockfd;
    struct sockaddr_in serv_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }
    sockfd_global = sockfd;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    // Spawn thread to receive messages
    pthread_t recv_tid;
    if (pthread_create(&recv_tid, NULL, recv_handler, &sockfd) != 0) {
        perror("pthread_create");
        close(sockfd);
        return 1;
    }
    pthread_detach(recv_tid);

    // Send username as first message
    if (send(sockfd, username, strlen(username), 0) <= 0) {
        perror("send username");
        close(sockfd);
        return 1;
    }

    // Read lines from stdin and send to server
    char input[BUF_SIZE];
    while (1) {
        if (!fgets(input, sizeof(input), stdin)) break;
        // remove newline
        input[strcspn(input, "\r\n")] = '\0';
        if (strcmp(input, "") == 0) continue;
        if (strcmp(input, "/quit") == 0 || strcmp(input, "quit") == 0) {
            printf("Exiting...\n");
            break;
        }
        ssize_t sent = send(sockfd, input, strlen(input), 0);
        if (sent <= 0) {
            perror("send");
            break;
        }
    }

    close(sockfd);
    return 0;
}

