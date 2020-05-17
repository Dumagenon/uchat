#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <sqlite3.h>
#include <regex.h>
#include "frozen.h"

#define MX_IN_ITOA(m) #m
#define MX_ITOA(m) MX_IN_ITOA(m)
#define MX_DB_USER "users.db"
#define MX_ROOMS_TABLE "CREATE TABLE ROOMS("  \
                       "ID                 INTEGER PRIMARY KEY NOT NULL," \
                       "NAME_ROOM          TEXT                NOT NULL, " \
                       "CUSTOMER_LOGIN     TEXT                NOT NULL);"
#define MX_USERS_TABLE "CREATE TABLE USERS(\
                        LOGIN          TEXT  UNIQUE   NOT NULL,\
                        PASSWORD       TEXT           NOT NULL,\
                        TOKEN          TEXT           NOT NULL,\
                        PERMISSION     INTEGER        NOT NULL);"
#define MX_MEMBER_TABLE "CREATE TABLE MEMBER("\
                        "ID_ROOM          INTEGER NOT NULL,"\
                        "LOGIN            TEXT    NOT NULL);"
#define MX_MESSAGE_TABLE "CREATE TABLE MESSAGE("  \
                         "ID_MESSAGE    INTEGER PRIMARY KEY NOT NULL," \
                         "LOGIN         TEXT                NOT NULL," \
                         "DATE          INTEGER             NOT NULL," \
                         "JSON          TEXT                NOT NULL);"

#define MX_LIST_BACK 0
#define MX_LOG_FILE "info.log"
#define MX_CONFIG "config.json"
#define MX_DEFAULT_CONFIG "{\n\n}\n"
#define MX_REQ_REGEX "([0-9]+[|][0-9]+[|]).+" 
#define MX_HASH_REGEX "^[a-f0-9]{32}$"
#define MX_LOGIN_REGEX "^[a-z0-9_-]{3,22}$"

#define MX_CERT_FILE "certificate.crt"
#define MX_KEY_FILE "private_key.pem"
#define MX_KEY_PASSWORD "12345678"

#define MX_BUF_SIZE 1024
#define MX_MD5_BUF_SIZE 32

typedef enum e_app_type {
    CLIENT,
    SERVER
}            t_app_type;

typedef struct s_message {
    unsigned int id_message;
    long long date;
    char *login;
    char *json;
}              t_message;

typedef struct s_members_room {
    char *login;
    struct s_members_room *next;
}              t_members_room;

typedef struct s_user {
    const char *token;
    const char *login;
    const char *password;
    unsigned int permission;
}              t_user;

typedef struct s_room {
    unsigned int id;
    char *name;
    char *customer;
}              t_room;


typedef struct s_ssl_con {
    SSL_CTX *ctx;
    SSL *ssl;
    char *cert_file;
    char *key_file;
    char *password;
}              t_ssl_con;

typedef struct s_node {
    void *data;
    struct s_node *next;
}              t_node;

typedef struct s_list {
    size_t size;
    t_node *head;
}              t_list;

typedef struct s_sockopt {
    int socket;
    int level;
    int option_name;
    const void *option_value;
    socklen_t option_len;
}              t_sockopt;

typedef enum e_logtype {
    LOGMSG,
    LOGWAR,
    LOGERR
}            t_logtype;

//Utils
int mx_match_search(char *str, char *regex);
void mx_randomize_str(char *str, size_t count);
void *mx_memdup(const void *mem, size_t size);


//wrappers
void *mx_malloc(size_t size);
void mx_free(void **ptr);
int mx_socket(int domain, int type, int protocol);
int mx_setsockopt(t_sockopt *sockopt);
int mx_close(int fd);
int mx_listen(int socket, int backlog);
int mx_accept(int socket, struct sockaddr *restrict address,
              socklen_t *restrict address_len);
int mx_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);
int mx_getaddrinfo(const char *hostname, const char *servname,
                   const struct addrinfo *hints, struct addrinfo **res);
int mx_pthread_mutex_init(pthread_mutex_t *mutex,
                          const pthread_mutexattr_t *attr);
int mx_pthread_mutex_destroy(pthread_mutex_t *mutex);
int mx_pthread_mutex_lock(pthread_mutex_t *mutex);
int mx_pthread_mutex_unlock(pthread_mutex_t *mutex);
FILE *mx_fopen(const char * restrict path, const char * restrict mode);
int mx_fclose(FILE *stream);

//list
void mx_push_node(t_list *list, void *data, size_t index);
void mx_remove_node(t_list *list, size_t index);
t_node *mx_new_node(void *data);
t_list *mx_new_list();
void mx_delete_list(t_list **list);

//logs
void mx_log_id(FILE *fd);
void mx_log_time(FILE *fd);
void mx_log_errno(FILE *fd);
void mx_log_type(FILE *fd, t_logtype type);
void mx_logger(const char *file, t_logtype type, const char *fmt, ...);
void mx_elogger(const char *file, t_logtype type, const char *fmt, ...);

//sqlite3
sqlite3 *mx_server_data_open(char *name_db);
void mx_close_database(sqlite3 *database);
void mx_create_table(sqlite3 *database, char *table);
void mx_free_user(t_user **user);
void mx_delete_room(sqlite3 *database, int id_room);
void mx_delete_user(sqlite3 *database, char *login);

t_user *mx_get_user_by_login(sqlite3 *database, char *login);
t_user *mx_get_user_by_token(sqlite3 *database, char *token);
t_message *mx_get_message_by_id(sqlite3 *database, int id_message);
t_message *mx_get_message_by_login(sqlite3 *database, char *login);
t_room *mx_get_room(sqlite3 *database,char *name_room);

void mx_update_permission_of_user(sqlite3 *database, unsigned int permission, char *login);
void mx_update_token(sqlite3 *database, char *new_token, char *login);

t_user *mx_insert_user(sqlite3 *database, char *login, char *password, char *token);
void mx_insert_message(sqlite3 *database, char *login, long long date, char *json);
void mx_insert_memeber(sqlite3 *database, int id_room, char *login);
t_room *mx_insert_room(sqlite3 *database, char *customer, char *name_room);

void mx_test_room();
void mx_test_member();
void mx_test_users();
void mx_test_message();
