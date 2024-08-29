
#include <gtk/gtk.h>
#include <stdlib.h>
//sudo apt-get install libgtk-3-dev

//gcc linux_editor.c -o text_editor `pkg-config --cflags --libs gtk+-3.0`

GtkWidget *window;
GtkWidget *text_view;
GtkTextBuffer *text_buffer;
char *current_file = NULL;

void on_open_button_clicked(GtkWidget *widget, gpointer data);
void on_save_button_clicked(GtkWidget *widget, gpointer data);
void load_text_file(const char *filename);
void save_text_file(const char *filename);

int main(int argc, char *argv[]) {
    GtkWidget *vbox;
    GtkWidget *open_button, *save_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Text Editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    text_view = gtk_text_view_new();
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    open_button = gtk_button_new_with_label("Open");
    save_button = gtk_button_new_with_label("Save");

    gtk_box_pack_start(GTK_BOX(vbox), open_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), save_button, FALSE, FALSE, 0);

    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open_button_clicked), NULL);
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_button_clicked), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

void on_open_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        current_file = filename;
        load_text_file(filename);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void on_save_button_clicked(GtkWidget *widget, gpointer data) {
    if (current_file) {
        save_text_file(current_file);
    } else {
        GtkWidget *dialog;
        dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE,
                                             "_Cancel", GTK_RESPONSE_CANCEL,
                                             "_Save", GTK_RESPONSE_ACCEPT,
                                             NULL);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            char *filename;
            GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
            filename = gtk_file_chooser_get_filename(chooser);

            save_text_file(filename);

            g_free(filename);
        }

        gtk_widget_destroy(dialog);
    }
}

void load_text_file(const char *filename) {
    gchar *content;
    gsize length;

    if (g_file_get_contents(filename, &content, &length, NULL)) {
        gtk_text_buffer_set_text(text_buffer, content, length);
        g_free(content);
    } else {
        g_print("Failed to read file %s\n", filename);
    }
}

void save_text_file(const char *filename) {
    GtkTextIter start, end;
    gchar *content;

    gtk_text_buffer_get_bounds(text_buffer, &start, &end);
    content = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    if (g_file_set_contents(filename, content, -1, NULL)) {
        g_print("File saved successfully\n");
    } else {
        g_print("Failed to save file %s\n", filename);
    }

    g_free(content);
}
