#include "protocol.h"

int mx_get_type_dtp(cJSON *json) {
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    int result = -1;

    if (!cJSON_IsNumber(type))
        return result;
    result = type->valueint;
    cJSON_Delete(type);
    return result;
}

void mx_free_request_struct(t_dtp **request) {
    if (request && *request) {
        cJSON_Delete((*request)->json);
        mx_free((void **)(&(*request)->data));
        mx_free((void**)request);
    }
}

t_dtp *mx_request_creation(char *req_body) {
    t_dtp *req_struct = mx_malloc(sizeof(t_dtp));
    size_t req_len = strlen(req_body);
    size_t buf_size = sizeof(int) + req_len + 1;
    char str[buf_size];

    bzero(str, buf_size);
    strcpy(str + 4, req_body);
    memcpy(str, &req_len, sizeof(int));
    req_struct->data = (char*)mx_memdup(str, buf_size);
    req_struct->str = req_struct->data + 4;
    req_struct->len = buf_size - 1;
    req_struct->json = cJSON_Parse(req_struct->str);
    if (!req_struct->json) {
        mx_logger(MX_LOG_FILE, LOGWAR, "Invalid json\n");
        return NULL;
    }
    return req_struct;
}
