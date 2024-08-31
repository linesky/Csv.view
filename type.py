
import sys

def main():
    # Verifica se o número de argumentos é suficiente
    print("\033c\033[47;30m")
    if len(sys.argv) != 2:
        print("Use: python type.py <file name>")
        return

    filename = sys.argv[1]

    try:
        # Abre o arquivo passado como argumento
        with open(filename, 'r') as file:
            # Lê o arquivo linha por linha e imprime na consola
            for line in file:
                print(line, end='')
    except FileNotFoundError:
        print(f"Erro: file '{filename}' not find.")
    except Exception as e:
        print(f"error: {e}")

if __name__ == "__main__":
    main()
