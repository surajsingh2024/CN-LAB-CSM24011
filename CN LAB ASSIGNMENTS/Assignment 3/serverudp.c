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
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[1024];

    // Fruit stock database
    struct Fruit fruits[3] = {
        {"Apple", 10, 0, "None"},
        {"Banana", 20, 0, "None"},
        {"Orange", 15, 0, "None"}
    };

    // List of unique customers
    char customers[MAX_CLIENTS][50];
    int customer_count = 0;

    // Creating UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4001);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("UDP Server running... waiting for clients.\n");

    while (1) {
        addr_size = sizeof(clientAddr);

        // Receive data from client
        recvfrom(sockfd, buffer, sizeof(buffer), 0,
                 (struct sockaddr *)&clientAddr, &addr_size);

        char client_id[50];
        sprintf(client_id, "%s:%d", inet_ntoa(clientAddr.sin_addr),
                ntohs(clientAddr.sin_port));

        // Check if the client is new
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

        // Extract fruit name and quantity
        char fruit_name[20];
        int req_qty;
        sscanf(buffer, "%s %d", fruit_name, &req_qty);

        int found = 0;

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
                        "Unique Customers: %d\n",
                        fruits[i].name, fruits[i].quantity, customer_count);
                } else {
                    sprintf(buffer,
                        "Sorry! Only %d %s left.\n"
                        "Unique Customers: %d\n",
                        fruits[i].quantity, fruits[i].name, customer_count);
                }
                break;
            }
        }

        if (!found) {
            sprintf(buffer, "Fruit not found!\n");
        }

        // Send reply
        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&clientAddr, addr_size);
    }

    close(sockfd);
    return 0;
}

