#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define DEFAULT_PORT 5001
#define MAX_CLIENTS 100
#define BUF_SIZE 1024
#define NAME_LEN 32
#define LOGFILE "log.txt"

typedef struct {
    int sockfd;
    char name[NAME_LEN];
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t logfile_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Add client to clients list */
void add_client(client_t *cl) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (!clients[i]) {
            clients[i] = cl;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

/* Remove client from clients list */
void remove_client(int sockfd) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i]) {
            if (clients[i]->sockfd == sockfd) {
                free(clients[i]);
                clients[i] = NULL;
                break;
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

/* Broadcast message to all clients except optional excluded sock (use -1 to send to all) */
void broadcast_message(const char *message, int exclude_sock) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i]) {
            if (clients[i]->sockfd != exclude_sock) {
                ssize_t sent = send(clients[i]->sockfd, message, strlen(message), 0);
                if (sent <= 0) {
                   
                }
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

/* Append message to server log file with timestamp */
void log_message(const char *sender, const char *message) {
    char timestr[64];
    time_t now = time(NULL);
    struct tm t;
    localtime_r(&now, &t);
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &t);

    pthread_mutex_lock(&logfile_mutex);
    FILE *f = fopen(LOGFILE, "a");
    if (f) {
        fprintf(f, "[%s] %s: %s\n", timestr, sender, message);
        fflush(f);
        fclose(f);
    } else {
        fprintf(stderr, "Unable to open log file %s: %s\n", LOGFILE, strerror(errno));
    }
    pthread_mutex_unlock(&logfile_mutex);
}

/* Thread routine for a connected client */
void *client_handler(void *arg) {
    client_t *cli = (client_t *)arg;
    char buff_in[BUF_SIZE];
    char message[BUF_SIZE + NAME_LEN + 32];

    // First message expected to be client's name (username)
    ssize_t rlen = recv(cli->sockfd, buff_in, NAME_LEN - 1, 0);
    if (rlen <= 0) {
        close(cli->sockfd);
        remove_client(cli->sockfd);
        pthread_exit(NULL);
    }
    buff_in[rlen] = '\0';
    // sanitize name
    buff_in[strcspn(buff_in, "\r\n")] = '\0';
    strncpy(cli->name, buff_in, NAME_LEN-1);
    cli->name[NAME_LEN-1] = '\0';

    // Announce join
    snprintf(message, sizeof(message), "*** %s has joined the chat ***\n", cli->name);
    printf("%s", message);
    broadcast_message(message, -1);
    log_message("SERVER", message);

    while (1) {
        ssize_t len = recv(cli->sockfd, buff_in, BUF_SIZE - 1, 0);
        if (len <= 0) {
            // client disconnected
            snprintf(message, sizeof(message), "*** %s has left the chat ***\n", cli->name);
            printf("%s", message);
            broadcast_message(message, cli->sockfd);
            log_message("SERVER", message);
            close(cli->sockfd);
            remove_client(cli->sockfd);
            break;
        }
        buff_in[len] = '\0';
        
        buff_in[strcspn(buff_in, "\r\n")] = '\0';

       
        snprintf(message, sizeof(message), "%s: %s\n", cli->name, buff_in);

        // Broadcast and log
        broadcast_message(message, -1);
        log_message(cli->name, buff_in);
    }

    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    if (argc >= 2) port = atoi(argv[1]);

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    // initialize clients array
    for (int i = 0; i < MAX_CLIENTS; ++i) clients[i] = NULL;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // allow reuse of address
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        close(listenfd);
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, 10) < 0) {
        perror("listen");
        close(listenfd);
        exit(EXIT_FAILURE);
    }

    printf("Chat server listening on port %d\n", port);
    printf("Messages will be logged to %s\n", LOGFILE);

    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &cli_len);
        if (connfd < 0) {
            perror("accept");
            continue;
        }

        // Create client structure
        client_t *cli = (client_t *)malloc(sizeof(client_t));
        if (!cli) {
            fprintf(stderr, "malloc failed\n");
            close(connfd);
            continue;
        }
        cli->sockfd = connfd;
        memset(cli->name, 0, NAME_LEN);

        // Add to clients list
        add_client(cli);

        // Create thread
        pthread_t tid;
        if (pthread_create(&tid, NULL, &client_handler, (void*)cli) != 0) {
            perror("pthread_create");
            remove_client(cli->sockfd);
            close(connfd);
            free(cli);
        } else {
            pthread_detach(tid);
        }
    }

    // cleanup (never reached in this simple server)
    close(listenfd);
    return 0;
}

