
import tkinter as tk
from tkinter import filedialog
from tkinter import ttk
import csv

def open_file():
    file_path = filedialog.askopenfilename(filetypes=[("CSV files", "*.csv")])
    if not file_path:
        return

    # Limpa a Treeview existente
    for item in tree.get_children():
        tree.delete(item)

    # Lê o arquivo CSV e insere os dados na Treeview
    with open(file_path, newline='', encoding='utf-8') as csvfiles:
        reader = csv.reader(csvfiles)
        headers = next(reader, None)  # Lê os cabeçalhos

        if headers:
            tree["columns"] = headers
            tree.heading("#0", text="", anchor="w")
            tree.column("#0", anchor="w", width=0, stretch=tk.NO)

            for header in headers:
                tree.heading(header, text=header, anchor="w")
                tree.column(header, anchor="w")

            for row in reader:
                tree.insert("", "end", values=row)

# Configuração da janela principal
root = tk.Tk()
root.title("CSV File Viewer")

# Botão para abrir arquivo CSV
open_button = tk.Button(root, text="Open", command=open_file)
open_button.pack(pady=10)

# Configuração da Treeview para exibir os dados do CSV
tree = ttk.Treeview(root)
tree.pack(fill="both", expand=True)

# Configuração de barras de rolagem para a Treeview
vsb = ttk.Scrollbar(root, orient="vertical", command=tree.yview)
vsb.pack(side='right', fill='y')
tree.configure(yscrollcommand=vsb.set)

hsb = ttk.Scrollbar(root, orient="horizontal", command=tree.xview)
hsb.pack(side='bottom', fill='x')
tree.configure(xscrollcommand=hsb.set)

# Inicia o loop principal do Tkinter
root.mainloop()
