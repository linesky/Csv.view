#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Verifica se o número de argumentos é suficiente
    printf("\033c\033[47;30m");
    if (argc != 2) {
        printf("Uso: type <file name>\n");
        return 1;
    }

    // Abre o arquivo passado como argumento
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Erro on file: %s\n", argv[1]);
        return 1;
    }

    char line[256];

    // Lê o arquivo linha por linha e imprime na consola
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    // Fecha o arquivo
    fclose(file);

    return 0;
}

