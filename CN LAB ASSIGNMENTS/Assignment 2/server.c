#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_CLIENTS 100

struct Fruit {
    char name[20];
    int quantity;
    int last_sold;
    char timestamp[40];
};

int main() {
    int sockfd, newsock;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[1024];

    // Fruit database
    struct Fruit fruits[3] = {
        {"Apple", 10, 0, "None"},
        {"Banana", 20, 0, "None"},
        {"Orange", 15, 0, "None"}
    };

    // Unique customer list
    char customers[MAX_CLIENTS][50];
    int customer_count = 0;

    // Creating server socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4001);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    listen(sockfd, 5);
    printf("Server running... Waiting for clients.\n");

    while (1) {
        addr_size = sizeof(clientAddr);
        newsock = accept(sockfd, (struct sockaddr *)&clientAddr, &addr_size);

        char client_id[50];
        sprintf(client_id, "%s:%d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // Check if customer is unique
        int is_new = 1;
        for (int i = 0; i < customer_count; i++) {
            if (strcmp(customers[i], client_id) == 0) {
                is_new = 0;
                break;
            }
        }
        if (is_new) {
            strcpy(customers[customer_count++], client_id);
        }

        // Receive data from client
        recv(newsock, buffer, sizeof(buffer), 0);
        char fruit_name[20];
        int req_qty;

        sscanf(buffer, "%s %d", fruit_name, &req_qty);

        int found = 0;
        int success = 0;

        for (int i = 0; i < 3; i++) {
            if (strcmp(fruits[i].name, fruit_name) == 0) {
                found = 1;

                if (fruits[i].quantity >= req_qty) {
                    fruits[i].quantity -= req_qty;
                    fruits[i].last_sold = req_qty;

                    time_t now = time(NULL);
                    strcpy(fruits[i].timestamp, ctime(&now));

                    sprintf(buffer,
                        "Purchase Successful!\n"
                        "Remaining %s: %d\n"
                        "Customers till now: %d\n",
                        fruits[i].name, fruits[i].quantity, customer_count
                    );

                    send(newsock, buffer, strlen(buffer), 0);
                } else {
                    sprintf(buffer,
                        "Sorry! Only %d %s left.\n"
                        "Customers till now: %d\n",
                        fruits[i].quantity, fruits[i].name, customer_count
                    );
                    send(newsock, buffer, strlen(buffer), 0);
                }
                break;
            }
        }

        if (!found) {
            sprintf(buffer, "Fruit not found!\n");
            send(newsock, buffer, strlen(buffer), 0);
        }

        close(newsock);
    }

    close(sockfd);
    return 0;
}

