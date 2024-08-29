#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//sudo apt-get install libgtk-3-dev

//gcc csv_editor.c -o csv_editor `pkg-config --cflags --libs gtk+-3.0`

#define MAX_COLUMNS 10
#define MAX_ROWS 100

GtkWidget *treeview;
GtkWidget *window;
GtkListStore *liststore;
char *current_file = NULL;
int num_columns = 0;

void load_csv(const char *filename);
void save_csv(const char *filename);
void on_open_button_clicked(GtkWidget *widget, gpointer data);
void on_save_button_clicked(GtkWidget *widget, gpointer data);

int main(int argc, char *argv[]) {
    GtkWidget *vbox;
    GtkWidget *open_button, *save_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CSV Editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    treeview = gtk_tree_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), treeview, TRUE, TRUE, 0);

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

void load_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        g_print("Could not open file %s\n", filename);
        return;
    }

    char line[1024];
    char *token;
    int row = 0;

    if (liststore) {
        g_object_unref(liststore);
    }

    liststore = gtk_list_store_new(MAX_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                   G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                   G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(liststore));

    // Read and set columns
    if (fgets(line, sizeof(line), file)) {
        num_columns = 0;
        token = strtok(line, ",");
        while (token) {
            GtkTreeViewColumn *column;
            GtkCellRenderer *renderer;

            renderer = gtk_cell_renderer_text_new();
            column = gtk_tree_view_column_new_with_attributes(token, renderer, "text", num_columns, NULL);
            gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

            num_columns++;
            token = strtok(NULL, ",");
        }
    }

    // Read and set rows
    while (fgets(line, sizeof(line), file) && row < MAX_ROWS) {
        GtkTreeIter iter;
        gtk_list_store_append(liststore, &iter);

        token = strtok(line, ",");
        for (int col = 0; col < num_columns; col++) {
            gtk_list_store_set(liststore, &iter, col, token ? token : "", -1);
            token = strtok(NULL, ",");
        }
        row++;
    }

    fclose(file);
}

void save_csv(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        g_print("Could not open file %s for writing\n", filename);
        return;
    }

    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(liststore), &iter);

    // Save columns headers
    for (int col = 0; col < num_columns; col++) {
        GtkTreeViewColumn *column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), col);
        const gchar *title = gtk_tree_view_column_get_title(column);
        fprintf(file, "%s", title);
        if (col < num_columns - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    // Save each row
    while (valid) {
        for (int col = 0; col < num_columns; col++) {
            gchar *text;
            gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter, col, &text, -1);
            fprintf(file, "%s", text);
            g_free(text);

            if (col < num_columns - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");

        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(liststore), &iter);
    }

    fclose(file);
    g_print("File saved successfully\n");
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
        load_csv(filename);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void on_save_button_clicked(GtkWidget *widget, gpointer data) {
    if (current_file) {
        save_csv(current_file);
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

            save_csv(filename);

            g_free(filename);
        }

        gtk_widget_destroy(dialog);
    }
}

