#include "utils.h"

void mx_free_user(t_user **user) {
    mx_free((void**)&(*user)->login);
    mx_free((void**)&(*user)->password); 
    mx_free((void**)&(*user)->token); 
    mx_free((void**)user); 
    *user = NULL;
}

void mx_free_message(t_message **message) {
    mx_free((void **)&(*message)->login);
    mx_free((void **)&(*message)->message);
    mx_free((void **)&(*message)->name_room);
    mx_free((void **)message);
    message = NULL;
}

void mx_free_room(t_room **room) {
    mx_free((void**)&(*room)->customer);
    mx_free((void**)&(*room)->name_room);
    mx_free((void**)room);
    room = NULL;
}
