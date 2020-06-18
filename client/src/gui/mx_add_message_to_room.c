#include "client.h"

static void add_message_row(t_gmsg *msg, GtkBuilder *builder) {
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *event = mx_create_message_row(msg);
    t_groom *room = mx_get_groom_by_id(msg->room_id, builder);
    GtkListBox *box = room->box_messages;
    t_signal_data *data = NULL;

    gtk_widget_set_can_focus(row, FALSE);
    gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), FALSE);
    data = mx_create_sigdata(builder, NULL, GTK_LIST_BOX_ROW(row));

    msg->row_msg = GTK_LIST_BOX_ROW(row);
    gtk_container_add(GTK_CONTAINER(row), event);
    g_signal_connect(event, "button_release_event",
                     G_CALLBACK(mx_select_msg), data);
    room->is_updated = true;
    gtk_list_box_insert(box, row, -1);
    gtk_list_box_row_changed(room->row_room);
    room->is_updated = false;
    gtk_widget_show_all(GTK_WIDGET(box));
    g_object_set_data_full(G_OBJECT(row), "gmsg", msg,
                           (GDestroyNotify)mx_delete_gmsg);
    g_object_set_data_full(G_OBJECT(event), "sigdata", data,
                           (GDestroyNotify)mx_free_sigdata);
}

void mx_add_message_to_room(t_gmsg *msg, GtkBuilder *builder) {
    add_message_row(msg, builder);
}
