#pragma once

#include "utils.h"
#include "protocol.h"
#include <sqlite3.h>

#define MX_LISTENQ 1024
#define MX_PORT_LEN 8
#define MX_SERVER_LOG_FILE "server.log"

typedef struct s_chat {
    int listen_fd;
    t_list *clients;
    socklen_t len;
    pthread_mutex_t mutex;
    sqlite3* database;
}              t_chat;

typedef struct s_client {
    struct sockaddr *cliaddr;
    pthread_t tid;
    socklen_t len;
    char ip[INET6_ADDRSTRLEN];
    char port[MX_PORT_LEN];
    int socket_fd;
    t_user *user;
    t_chat *chat;
    SSL *ssl;
}              t_client;

//data protocol handler functions
bool mx_log_in(t_dtp *login, t_client *client);
bool mx_sign_up(t_dtp *login, t_client *client);
bool mx_log_in_token(t_dtp *token, t_client *client);
bool mx_authorization(t_client *client, t_dtp *data);

int mx_tcp_listen(const char *serv, socklen_t *addr_len);
void mx_get_client_info(t_client *client);
t_client *mx_new_client(socklen_t len);
t_chat *mx_init_chat(int argc, char **argv);
void mx_deinit_chat(t_chat **chat);
void mx_connect_client(t_client *client);
void mx_delete_client_list(t_list *list, t_client *client);
void mx_delete_client(t_client **client);
