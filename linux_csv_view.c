#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//gcc csv_viewer.c -o csv_viewer `pkg-config --cflags --libs gtk+-3.0`
#define MAX_COLUMNS 20

// Função para carregar dados CSV no modelo de lista do TreeView
void load_csv(GtkWidget *treeview, const char *filename) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *file;
    char line[1024];
    char *token;
    int column_count = 0;
    char *columns[MAX_COLUMNS];

    // Tentar abrir o arquivo
    file = fopen(filename, "r");
    if (!file) {
        g_print("Failed to open file: %s\n", filename);
        return;
    }

    // Ler o cabeçalho para determinar o número de colunas
    if (fgets(line, sizeof(line), file)) {
        token = strtok(line, ",");
        while (token && column_count < MAX_COLUMNS) {
            columns[column_count++] = g_strdup(token);
            token = strtok(NULL, ",");
        }
    }

    // Criar um modelo de lista com as colunas identificadas
    GType types[MAX_COLUMNS];
    for (int i = 0; i < column_count; i++) {
        types[i] = G_TYPE_STRING;
    }
    store = gtk_list_store_newv(column_count, types);

    // Ler as linhas de dados e adicionar ao modelo
    while (fgets(line, sizeof(line), file)) {
        gtk_list_store_append(store, &iter);
        token = strtok(line, ",");
        for (int i = 0; i < column_count && token; i++) {
            gtk_list_store_set(store, &iter, i, token, -1);
            token = strtok(NULL, ",");
        }
    }

    fclose(file);

    // Configurar o TreeView para usar o novo modelo de lista
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

    // Criar colunas no TreeView
    for (int i = 0; i < column_count; i++) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(columns[i], renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        g_free(columns[i]);  // Liberar memória
    }

    g_object_unref(store);
}

// Callback para o botão "Open"
void on_open_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkWidget *treeview = (GtkWidget *)user_data;
    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                         GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        load_csv(treeview, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

// Função principal para inicializar a aplicação GTK
int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *button;
    GtkWidget *scrolled_window;
    GtkWidget *treeview;

    gtk_init(&argc, &argv);

    // Criar a janela principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CSV File Viewer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Criar um botão "Open"
    button = gtk_button_new_with_label("Open");
    g_signal_connect(button, "clicked", G_CALLBACK(on_open_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    // Criar uma janela de rolagem e um TreeView para exibir o CSV
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    treeview = gtk_tree_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);

    g_signal_connect(button, "clicked", G_CALLBACK(on_open_clicked), treeview);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

