#include "server.h"

t_dtp *mx_search_msgs_request(cJSON *array) {
    cJSON *res = cJSON_CreateObject();

    if (!cJSON_AddNumberToObject(res, "type", RQ_SEARCH_MSG))
        return NULL;
    if (!cJSON_AddItemReferenceToObject(res, "msgs", array))
        return NULL;
    return mx_get_transport_data(res);
}

bool mx_search_msgs_handler(t_dtp *data, t_client *client) {
    cJSON *msg = cJSON_GetObjectItemCaseSensitive(data->json, "msg");
    cJSON *room_id = cJSON_GetObjectItemCaseSensitive(data->json, "room_id");
    t_dtp *answer = NULL;

    if (!cJSON_IsString(msg))
        return false;
    if (!cJSON_IsNumber(room_id))
        return false;
    if (!mx_is_member(client->info->database, client->user->user_id, room_id->valueint))
        return false;
    if (mx_get_type_member(client->info->database, client->user->user_id, room_id->valueint) == DB_BANNED)
        return false;
    answer = mx_search_msgs_request(mx_search_message(client->info->database, msg->valuestring, room_id->valueint));
    mx_send(client->out, answer);
    mx_free_request(&answer);
    return true;
}