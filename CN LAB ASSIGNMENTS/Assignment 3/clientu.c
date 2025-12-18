#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char buffer[1024], fruit[20];
    int qty;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4001);
    serverAddr.sin_addr.s_addr = inet_addr("10.0.0.1"); // server IP

    printf("Enter fruit name: ");
    scanf("%s", fruit);

    printf("Enter quantity: ");
    scanf("%d", &qty);

    sprintf(buffer, "%s %d", fruit, qty);

    addr_size = sizeof(serverAddr);

    // Send request
    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&serverAddr, addr_size);

    // Receive response
    recvfrom(sockfd, buffer, sizeof(buffer), 0,
             (struct sockaddr *)&serverAddr, &addr_size);

    printf("Server Response:\n%s\n", buffer);

    close(sockfd);
    return 0;
}

