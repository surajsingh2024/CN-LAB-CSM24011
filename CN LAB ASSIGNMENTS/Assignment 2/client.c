#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[1024], fruit[20];
    int qty;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4001);
    serverAddr.sin_addr.s_addr = inet_addr("10.0.0.1");  // server IP in mininet

    connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("Enter fruit name: ");
    scanf("%s", fruit);

    printf("Enter quantity: ");
    scanf("%d", &qty);

    sprintf(buffer, "%s %d", fruit, qty);
    send(sockfd, buffer, strlen(buffer), 0);

    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server Response:\n%s\n", buffer);

    close(sockfd);
    return 0;
}

