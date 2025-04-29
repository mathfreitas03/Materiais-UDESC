#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int percorrer(int **matriz, int x, int y, int altura_matriz, int largura_matriz){

    if (x < 0 || x >= altura_matriz || y < 0 || y >= largura_matriz) {
        return 0;
    }    
    if (matriz[x][y] != 0) {
        return 0;
    }

    if (x == altura_matriz - 1 && y == largura_matriz - 1 && matriz[x][y] == 0) {
        matriz[x][y] = 2; // marca o destino
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


int main(int argc, char const *argv[])
{
    FILE *lab = fopen("lab.txt", "r");

    char buffer[1024];
    char c;
    
    int linhas = 0;
    int colunas = 0;
    int i = 0;
    int j = 0;

    while(fgets(buffer, 1024, lab)){
        colunas = 0;
        while(buffer[colunas] != '\n'){
            colunas++;
        }
        linhas++;
    }

    int **labirinto = malloc(linhas * sizeof(int));

    for(int i = 0; i < linhas; i++){
        labirinto[i] = malloc((colunas) * sizeof(int)); 
    }
    
    rewind(lab);
    
    i = 0;
    j = 0;

    while(fgets(buffer, 1024, lab)){
        i = 0;
        while(buffer[i] != '\n'){
            labirinto[i][j] = (int) (buffer[i] - '0');
            i++;
        }
        j++;
    }
    
    percorrer(labirinto, 0, 0, linhas, colunas);

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            if(labirinto[i][j] == 1){
                printf("\033[47m \033[0m"); 
            }
            else if(labirinto[i][j] == 2){
                printf("x"); 
            }
            else
                printf(" ");
        }
        printf("\n");
    }

    // printf("\033[47m \033[0m\n"); 

    fclose(lab);

    return 0;
}
