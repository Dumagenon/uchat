#include  "server.h"

void mx_insert_contact(sqlite3 *db, guint64 user_id, guint64 contact_id,
                       gint8 type) {
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    gchar *request = NULL;

    sqlite3_str_appendf(sqlite_str, "insert into contacts(user_id, contact_id,"
                                    " type)(%llu, %llu, %d)", user_id, 
                        contact_id, type);
    request = sqlite3_str_finish(sqlite_str);
    mx_error_sqlite(sqlite3_exec(db, request, 0, 0, 0), "exec", "insert cont");
    sqlite3_free(request);
}

void mx_get_contact_() {

}

