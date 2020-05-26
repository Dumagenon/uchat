#include "client.h"

// static int sockfd;
static FILE *fp;
// static int done;

bool is_valid_login(char *login) {
    if (!mx_match_search(login, MX_LOGIN_REGEX)) {
        printf("Login must countain \'-\' and \'_\'chars and have 3-22 length\n");
        return false;
    }
    return true;
}

void mx_signup(SSL *ssl) {
    char login[1024] = {0};
    char password[1024] = {0};
    char md5_password[MX_MD5_BUF_SIZE + 1];
    t_dtp *signup_request = NULL;
    bool valid = false;

    while (!valid) {
        printf("Enter your login: ");
        fgets(login, 1024, fp);
        printf("Enter your password: ");
        fgets(password, 1024, fp);
        password[strlen(password) - 1] = '\0';
        login[strlen(login) - 1] = '\0';
        valid = is_valid_login(login);
    }
    mx_md5(md5_password, (const unsigned char*)password, strlen(password));
    signup_request = mx_sign_up_request(login, md5_password);
    mx_send(ssl, signup_request);
    mx_free_request_struct(&signup_request);
}

void mx_login(SSL *ssl) {
    char login[1024] = {0};
    char password[1024] = {0};
    char md5_password[MX_MD5_BUF_SIZE + 1];
    t_dtp *login_request = NULL;
    bool valid = false;

    while (!valid) {
        printf("Enter your login: ");
        fgets(login, 1024, fp);
        printf("Enter your password: ");
        fgets(password, 1024, fp);
        password[strlen(password) - 1] = '\0';
        login[strlen(login) - 1] = '\0';
        valid = is_valid_login(login);
    }
    mx_md5(md5_password, (const unsigned char*)password, strlen(password));
    login_request = mx_log_in_request(login, md5_password);
    mx_send(ssl, login_request);
    mx_free_request_struct(&login_request);
}

// void *copyto(void *arg) {
//     char sendline[1024];
//     t_dtp *request = NULL;
//     SSL *ssl = (SSL*)arg;
//     system("leaks -q uchat");
//     while (fgets(sendline, 1024, fp)) {
//         if (!strcmp("signup\n", sendline))
//             mx_signup(ssl);
//         else if (!strcmp("login\n", sendline))
//             mx_login(ssl);
//         else {
//             request = mx_msg_request(1, NULL, sendline);
//             // printf("req = %s len = %zu len = %zu\n", request->str, strlen(request->str), request->len);
//             mx_send(ssl, request);
//             mx_free_request_struct(&request);
//             bzero(sendline, sizeof(sendline));
//         }
//     }
//     shutdown(sockfd, SHUT_WR);
//     done = 1;
//     return NULL;
// }

// void str_cli(FILE *fp_arg, SSL *ssl) {
//     // pthread_t tid;
//     char buf[1025];
//     t_dtp *dtp = NULL;

//     bzero(&buf, sizeof(buf));
//     fp = fp_arg;
//     // mx_pthread_create(&tid, NULL, copyto, ssl);
//     while ((dtp = mx_recv(ssl))) {
//         printf("%s\n", dtp->str);
//         mx_free_request_struct(&dtp);
//     }

//     if (done == 0)
//         exit(1);
// }

static void change_working_dir(void) {
    #ifdef MX_CLIENT
    if (chdir(MX_CLIENT)) {
        mx_elogger(NULL, LOGERR,
                   "No working directory %s\n", MX_CLIENT);
    }
    #else
    mx_elogger(NULL, LOGERR, "No working directory");
    #endif
}

int main(int argc, char **argv) {
    int sockfd;
    t_ssl_con *ssl = NULL;
    t_chat *chat = mx_init_chat();

    change_working_dir();
    if (argc != 3) {
        printf("usage\n");
        exit(1);
    }
    ssl = mx_init_ssl(CLIENT);
    mx_logger(MX_LOG_FILE, LOGMSG, "started client: %s %s\n", argv[1], argv[2]);
    sockfd = mx_tcp_connect(argv[1], argv[2]);
    ssl->ssl = SSL_new(ssl->ctx);
    chat->ssl = ssl->ssl;
    SSL_set_fd(ssl->ssl, sockfd);
    if (SSL_connect(ssl->ssl) == -1) {
        mx_elogger(MX_LOG_FILE, LOGERR, "SSL_connect failded\n");
    }
    chat->builder = mx_init_window(argc, argv);
    mx_init_receiver(chat);
    mx_start_window(chat);
}
