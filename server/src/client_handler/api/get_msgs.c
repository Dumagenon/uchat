#include "server.h"

bool mx_get_msgs_handler(t_dtp *data, t_client *client) {
    cJSON *date = cJSON_GetObjectItemCaseSensitive(data->json, "date");
    cJSON *count = cJSON_GetObjectItemCaseSensitive(data->json, "count");
    cJSON *room_id = cJSON_GetObjectItemCaseSensitive(data->json, "room_id");
    t_dtp *msgs = NULL;
    cJSON *msgs_json = NULL;

    if (!cJSON_IsNumber(date))
        return false;
    if (!cJSON_IsNumber(count))
        return false;
    if (!cJSON_IsNumber(room_id))
        return false;
    if (!mx_is_member(client->info->database, client->user->user_id, room_id->valueint))
        return false;
    if (mx_get_type_member(client->info->database, client->user->user_id, room_id->valueint) == DB_BANNED)
        return false;
    msgs_json = mx_get_new_messages_by_id(client->info->database, room_id->valueint, date->valueint, count->valueint);
    if (!cJSON_AddNumberToObject(msgs_json, "type", RQ_GET_NEW_MSGS)) {
        cJSON_Delete(msgs_json);
        return false;
    }
    msgs = mx_get_transport_data(msgs_json);
    mx_send(client->out, msgs);
    mx_free_request(&msgs);
    return true;
}
