#include "server.h"
#include <sqlite3.h>

int callback(void *message, int argc, char** argv, char **data_parametr) {

    for (int i = 0; i < argc; i++) {
        printf("%s -> %s\n", data_parametr[i], argv[i]);
        if (strcmp("MESSAGE", data_parametr[i]) == 0 && argv[i] != NULL) {
            message = strdup(argv[i]);
        }
    }
    printf("Ok\n");
    return 0;
}

int main(int argc, char **argv) {
    // sqlite3 *database = mx_server_data_open(MX_DB_USER);
    // mx_create_table_user(database);
    // t_user *user = mx_insert_user("admin1", "123", "token", database);
    // printf("ID -> %d\n", user->id);
    // printf("login -> %s\n", user->login);
    // printf("pass -> %s\n", user->password);
    // printf("token -> %s\n", user->token);
    // user = mx_insert_user("admin2", "1234", "token", database);
    // printf("ID -> %d\n", user->id);
    // printf("login -> %s\n", user->login);
    // printf("pass -> %s\n", user->password);
    // printf("token -> %s\n", user->token);
    // user = mx_insert_user("admin3", "12345", "token", database);
    // printf("ID -> %d\n", user->id);
    // printf("login -> %s\n", user->login);
    // printf("pass -> %s\n", user->password);
    // printf("token -> %s\n", user->token);
    // user = mx_get_user_on_login("admin3", database);
    // printf("ID -> %d\n", user->id);
    // printf("login -> %s\n", user->login);
    // printf("pass -> %s\n", user->password);
    // printf("token -> %s\n", user->token);
    // exit(1);
    t_chat *chat = mx_init_chat(argc, argv);
    chat->database = mx_server_data_open(MX_DB_USER);
    t_client *client = NULL;
    t_ssl_con *ssl = mx_init_ssl(SERVER);

    mx_create_table_user(chat->database);
    mx_logger(MX_LOG_FILE, LOGMSG,"started server pid[%d]: %s %s\n", getpid(), argv[0], argv[1]);
    while (1) {
        client = mx_new_client(chat->len);
        client->socket_fd = mx_accept(chat->listen_fd,
                                      client->cliaddr, &client->len);
        ssl->ssl = SSL_new(ssl->ctx);
        SSL_set_fd(ssl->ssl, client->socket_fd);
        if (SSL_accept(ssl->ssl) == -1)
            mx_elogger(MX_LOG_FILE, LOGERR, "ssl_accept\n");
        client->chat = chat;
        client->ssl = ssl->ssl;
        mx_connect_client(client);
    }
    mx_deinit_chat(&chat);
}
