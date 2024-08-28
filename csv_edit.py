
import tkinter as tk
from tkinter import filedialog, messagebox
import pandas as pd
#pip install pandas
class CSVEditor:
    def __init__(self, root):
        self.root = root
        self.root.title("CSV File Editor")

        # Frame para botões
        button_frame = tk.Frame(self.root)
        button_frame.pack(side=tk.TOP, fill=tk.X)

        # Botão Open
        open_button = tk.Button(button_frame, text="Open", command=self.open_file)
        open_button.pack(side=tk.LEFT, padx=5, pady=5)

        # Botão Save
        save_button = tk.Button(button_frame, text="Save", command=self.save_file)
        save_button.pack(side=tk.LEFT, padx=5, pady=5)

        # Text widget para mostrar e editar dados CSV
        self.text = tk.Text(self.root)
        self.text.pack(expand=True, fill=tk.BOTH)

        self.csv_file = None  # Variável para armazenar o caminho do arquivo CSV atual

    def open_file(self):
        # Abrir diálogo de arquivo para selecionar um CSV
        self.csv_file = filedialog.askopenfilename(
            title="Open CSV File",
            filetypes=(("CSV Files", "*.csv"), ("All Files", "*.*"))
        )

        if self.csv_file:
            try:
                # Carregar CSV usando pandas
                df = pd.read_csv(self.csv_file)
                self.text.delete(1.0, tk.END)  # Limpar o widget de texto
                self.text.insert(tk.END, df.to_string(index=False))  # Mostrar o conteúdo do CSV
            except Exception as e:
                messagebox.showerror("Error", f"Could not read file:\n{e}")

    def save_file(self):
        if not self.csv_file:
            messagebox.showwarning("Warning", "No file opened.")
            return

        try:
            # Obter conteúdo do widget de texto
            content = self.text.get(1.0, tk.END)
            # Converter de volta para DataFrame
            lines = content.split("\n")
            data = [line.split() for line in lines if line.strip()]
            df = pd.DataFrame(data[1:], columns=data[0])

            # Salvar CSV usando pandas
            df.to_csv(self.csv_file, index=False)
            messagebox.showinfo("Success", "File saved successfully!")
        except Exception as e:
            messagebox.showerror("Error", f"Could not save file:\n{e}")

# Inicializar a aplicação
root = tk.Tk()
app = CSVEditor(root)
root.mainloop()
