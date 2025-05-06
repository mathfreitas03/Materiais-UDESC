#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int percorrer(int **matriz, int x, int y, int altura_matriz, int largura_matriz) {
    if (x < 0 || x >= altura_matriz || y < 0 || y >= largura_matriz)
        return 0;

    if (matriz[x][y] != 0)
        return 0;

    if (x == altura_matriz - 1 && matriz[x][y] == 0) {
        matriz[x][y] = 2;
        return 1;
    }

    matriz[x][y] = 2;

    if (percorrer(matriz, x + 1, y, altura_matriz, largura_matriz) ||
        percorrer(matriz, x - 1, y, altura_matriz, largura_matriz) ||
        percorrer(matriz, x, y + 1, altura_matriz, largura_matriz) ||
        percorrer(matriz, x, y - 1, altura_matriz, largura_matriz)) {
        return 1;
    }

    matriz[x][y] = 0;
    return 0;
}

int main() {
    FILE *lab = fopen("lab.txt", "r");
    if (lab == NULL) {
        printf("Erro ao abrir o arquivo");
        return 1;
    }

    char buffer[1024];
    int linhas = 0, colunas = 0;

    while (fgets(buffer, sizeof(buffer), lab)) {
        if (linhas == 0) {
            while (buffer[colunas] != '\n' && buffer[colunas] != '\0') {
                colunas++;
            }
        }
        linhas++;
    }

    int **labirinto = malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        labirinto[i] = malloc(colunas * sizeof(int));
    }

    rewind(lab);

    int j = 0;
    while (fgets(buffer, sizeof(buffer), lab)) {
        for (int i = 0; i < colunas && buffer[i] != '\0' && buffer[i] != '\n'; i++) {
            labirinto[j][i] = buffer[i] - '0';
        }
        j++;
    }

    percorrer(labirinto, 0, 0, linhas, colunas);

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (labirinto[i][j] == 1) {
                printf("\033[47m \033[0m"); 
            } else if (labirinto[i][j] == 2) {
                printf("x");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    for (int i = 0; i < linhas; i++) {
        free(labirinto[i]);
    }
    free(labirinto);
    fclose(lab);

   system("pause");

    return 0;
}
